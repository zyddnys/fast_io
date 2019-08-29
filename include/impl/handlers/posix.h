#pragma once
#include<unistd.h>
#include<fcntl.h>
#include<stdexcept>
#include<string_view>
#include<cstddef>
#include<type_traits>
#include"../mode.h"
#include"../concept.h"
#include"../seek.h"
#include<system_error>
#include<tuple>
#include<memory>

namespace fast_io
{
	
namespace details
{
inline constexpr int calculate_posix_open_mode(open::mode const &om)
{
	using namespace open;
	std::size_t value(remove_ate(om).value);
	int mode(0);
	if(value&binary.value)
	{
#ifdef O_BINARY
		mode |= O_BINARY;
#endif
		value &= ~binary.value;
	}
	if(value&excl.value)
	{
		mode |= O_CREAT  | O_EXCL;
		value &= ~excl.value;
	}
	if(value&trunc.value)
	{
		mode |= O_TRUNC;
		value &= ~trunc.value;
	}
	switch(value)
	{
//Action if file already exists;	Action if file does not exist;	c-style mode;	Explanation
//Read from start;	Failure to open;	"r";	Open a file for reading
	case in:
		return mode | O_RDONLY;
//Destroy contents;	Create new;	"w";	Create a file for writing
	case out:
		return mode | O_WRONLY | O_CREAT | O_TRUNC;
//Append to file;	Create new;	"a";	Append to a file
	case app:
	case out|app:
		return mode | O_WRONLY | O_CREAT | O_APPEND;
//Read from start;	Error;	"r+";		Open a file for read/write
	case out|in:
		return mode | O_RDWR;
//Write to end;	Create new;	"a+";	Open a file for read/write
	case out|in|app:
	case in|app:
		return mode | O_RDWR | O_CREAT | O_APPEND;
//Destroy contents;	Error;	"wx";	Create a file for writing
	default:
		throw std::runtime_error("unknown posix file openmode");
	}
}
template<std::size_t om>
struct posix_file_openmode
{
	static int constexpr mode = calculate_posix_open_mode(om);
};
}

class posix_io_handle
{
	int fd;
protected:
	auto& protected_native_handle()
	{
		return fd;
	}
public:
	using char_type = char;
	using native_handle_type = int;
	native_handle_type native_handle() const
	{
		return fd;
	}
	posix_io_handle(int fdd):fd(fdd){}
	template<typename ContiguousIterator>
	ContiguousIterator read(ContiguousIterator begin,ContiguousIterator end)
	{
		auto read_bytes(::read(fd,std::addressof(*begin),std::addressof(*end)-std::addressof(*begin)));
		if(read_bytes==-1)
			throw std::system_error(errno,std::system_category());
		return begin+(read_bytes/sizeof(*begin));
	}
	template<typename ContiguousIterator>
	ContiguousIterator write(ContiguousIterator begin,ContiguousIterator end)
	{
		auto write_bytes(::write(fd,std::addressof(*begin),std::addressof(*end)-std::addressof(*begin)));
		if(write_bytes==-1)
			throw std::system_error(errno,std::system_category());
		return begin+(write_bytes/sizeof(*begin));
	}
	template<typename T>
	void seek(seek_type_t<T>,Integral i,seekdir s=seekdir::beg)
	{
		if(::lseek64(fd,seek_precondition<off64_t,T,char_type>(i),static_cast<int>(s))==-1)
			throw std::system_error(errno,std::system_category()); 
	}
	void seek(Integral i,seekdir s=seekdir::beg)
	{
		seek(seek_type<char_type>,i,s);
	}
	void flush()
	{
		// no need fsync. OS can deal with it
//		if(::fsync(fd)==-1)
//			throw std::system_error(errno,std::system_category());
	}
};

class posix_file:public posix_io_handle
{
	void close_impl() noexcept
	{
		if(native_handle()!=-1)
			close(native_handle());
	}
public:
	using char_type = posix_io_handle::char_type;
	using native_handle_type = posix_io_handle::native_handle_type;
	template<typename ...Args>
	posix_file(native_handle_t,Args&& ...args):posix_io_handle(std::forward<Args>(args)...)
	{
		if(native_handle()==-1)
			throw std::system_error(errno,std::generic_category());
	}
	template<typename ...Args>
	posix_file(native_interface_t,Args&& ...args):posix_file(fast_io::native_handle,::open(std::forward<Args>(args)...))
	{
	}
	template<std::size_t om>
	posix_file(std::string_view file,open::interface_t<om>):posix_file(native_interface,file.data(),details::posix_file_openmode<om>::mode,420)
	{
		if constexpr (with_ate(open::mode(om)))
			seek(0,seekdir::end);
	}
	//potential support modification prv in the future
	posix_file(std::string_view file,open::mode const& m):posix_file(native_interface,file.data(),details::calculate_posix_open_mode(m),420)
	{
		if(with_ate(m))
			seek(0,seekdir::end);
	}
	posix_file(std::string_view file,std::string_view mode):posix_file(file,fast_io::open::c_style(mode)){}
	posix_file(posix_file const&)=delete;
	posix_file& operator=(posix_file const&)=delete;
	posix_file(fast_io::posix_file&& b) noexcept : posix_io_handle(b.protected_native_handle())
	{
		b.protected_native_handle() = -1;
	}
	posix_file& operator=(posix_file&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			close_impl();
			protected_native_handle()=b.protected_native_handle();
			b.protected_native_handle() = -1;
		}
		return *this;
	}
	~posix_file()
	{
		close_impl();
	}
};
}