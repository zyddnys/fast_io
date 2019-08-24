#pragma once
#include<unistd.h>
#include<fcntl.h>
#include<stdexcept>
#include<string_view>
#include<cstddef>
#include<type_traits>
#include"../mode.h"
#include"../concept.h"
#include<system_error>
#include<tuple>

namespace fast_io
{
	
namespace details
{
inline constexpr int calculate_posix_open_mode(open::mode const &om)
{
	using namespace open;
	switch(remove_binary(remove_ate(om)))
	{
//Action if file already exists;	Action if file does not exist;	c-style mode;	Explanation
//Read from start;	Failure to open;	"r";	Open a file for reading
	case in:
		return {O_RDONLY};
//Destroy contents;	Create new;	"w";	Create a file for writing
	case out:
	case out|trunc:
		return {O_WRONLY | O_CREAT | O_TRUNC};
//Append to file;	Create new;	"a";	Append to a file
	case app:
	case out|app:
		return {O_WRONLY | O_CREAT | O_APPEND};
//Read from start;	Error;	"r+";		Open a file for read/write
	case out|in:
		return {O_RDWR};
//Destroy contents;	Create new;	"w+";	Create a file for read/write
	case out|in|trunc:
		return {O_RDWR | O_CREAT | O_TRUNC};
//Write to end;	Create new;	"a+";	Open a file for read/write
	case out|in|app:
	case in|app:
		return {O_RDWR | O_CREAT | O_APPEND};
//Destroy contents;	Error;	"wx";	Create a file for writing
	case out|no_overwrite:
	case out|trunc|no_overwrite:
		return {O_RDWR | O_CREAT | O_APPEND | O_EXCL};
//Append to file;	Error;	"ax";	Append to a file
	case app|no_overwrite:
	case out|app|no_overwrite:
		return {O_WRONLY | O_CREAT | O_APPEND | O_EXCL};
//Destroy contents;	Error;	"w+x";	Create a file for read/write
	case out|in|trunc|no_overwrite:
		return {O_RDWR | O_CREAT | O_TRUNC | O_EXCL};
//Write to end;	Error;	"a+x";	Open a file for read/write
	case out|in|app|no_overwrite:
	case in|app|no_overwrite:
		return {O_RDWR | O_CREAT | O_APPEND | O_EXCL};
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
	using traits_type = std::char_traits<char>;
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
	void flush()
	{
		if(::fsync(fd)==-1)
			throw std::system_error(errno,std::system_category());
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
	using traits_type = posix_io_handle::traits_type;
	using native_handle_type = posix_io_handle::native_handle_type;
	template<typename ...Args>
	posix_file(native_interface_t,Args&& ...args):posix_io_handle(::open(std::forward<Args>(args)...))
	{
		if(native_handle()==-1)
			throw std::system_error(errno,std::generic_category());
	}
	template<std::size_t om>
	posix_file(std::string_view file,open::interface_t<om>):posix_file(native_interface,file.data(),details::posix_file_openmode<om>::mode,644){}		//potential support modification prv in the future
	posix_file(std::string_view file,open::mode const& m):posix_file(native_interface,file.data(),details::calculate_posix_open_mode(m),644){}		//potential support modification prv in the future
	posix_file(std::string_view file,std::string_view mode):posix_file(file,fast_io::open::c_style(mode)){}
	posix_file(posix_file const&)=delete;
	posix_file& operator=(posix_file const&)=delete;
	posix_file(fast_io::posix_file&& b) noexcept : posix_io_handle(b.protected_native_handle())
	{
		b.protected_native_handle() = -1;
	}
	posix_file& operator=(posix_file&& b) noexcept
	{
		if(&b!=this)
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