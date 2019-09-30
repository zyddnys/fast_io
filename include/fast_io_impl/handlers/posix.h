#pragma once
#include<unistd.h>
#include<fcntl.h>

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
	void close_impl() noexcept
	{
		if(native_handle()!=-1)
			close(native_handle());
	}
public:
	using char_type = char;
	using native_handle_type = int;
	native_handle_type native_handle() const
	{
		return fd;
	}
	posix_io_handle() = default;
	posix_io_handle(int fdd):fd(fdd){}
	template<typename ContiguousIterator>
	ContiguousIterator read(ContiguousIterator begin,ContiguousIterator end)
	{
		auto read_bytes(::read(fd,std::addressof(*begin),(end-begin)*sizeof(*begin)));
		if(read_bytes==-1)
			throw std::system_error(errno,std::generic_category());
		return begin+(read_bytes/sizeof(*begin));
	}
	template<typename ContiguousIterator>
	ContiguousIterator write(ContiguousIterator begin,ContiguousIterator end)
	{
		auto write_bytes(::write(fd,std::addressof(*begin),(end-begin)*sizeof(*begin)));
		if(write_bytes==-1)
			throw std::system_error(errno,std::generic_category());
		return begin+(write_bytes/sizeof(*begin));
	}
	template<typename T>
	void seek(seek_type_t<T>,std::integral i,seekdir s=seekdir::beg)
	{
		if(::lseek64(fd,seek_precondition<off64_t,T,char_type>(i),static_cast<int>(s))==-1)
			throw std::system_error(errno,std::generic_category()); 
	}
	void seek(std::integral i,seekdir s=seekdir::beg)
	{
		seek(seek_type<char_type>,i,s);
	}
	void flush()
	{
		// no need fsync. OS can deal with it
//		if(::fsync(fd)==-1)
//			throw std::system_error(errno,std::generic_category());
	}

	posix_io_handle(posix_io_handle const& dp):fd(dup(dp.fd))
	{
		if(fd<0)
			throw std::system_error(errno,std::generic_category());
	}
	posix_io_handle& operator=(posix_io_handle const& dp)
	{
		auto newfd(dup2(dp.fd,fd));
		if(newfd<0)
			throw std::system_error(errno,std::generic_category());
		fd=newfd;
		return *this;
	}
	posix_io_handle(posix_io_handle&& b) noexcept : posix_io_handle(b.fd)
	{
		b.fd = -1;
	}
	posix_io_handle& operator=(posix_io_handle&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			close_impl();
			fd=b.fd;
			b.fd = -1;
		}
		return *this;
	}
};

class posix_file:public posix_io_handle
{
public:
	using char_type = posix_io_handle::char_type;
	using native_handle_type = posix_io_handle::native_handle_type;
	template<typename ...Args>
	posix_file(native_interface_t,Args&& ...args):posix_io_handle(::open(std::forward<Args>(args)...))
	{
		if(native_handle()==-1)
			throw std::system_error(errno,std::generic_category());
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
	~posix_file()
	{
		posix_io_handle::close_impl();
	}
};

class posix_pipe
{
public:
	using char_type = char;
	using native_handle_type = std::array<int,2>;
private:
	native_handle_type pipes;
	void close_impl()
	{
		for(auto const & e : pipes)
			if(e!=-1)
				close(e);
	}
public:
	posix_pipe()
	{
#ifdef _WIN32_WINNT
		if(_pipe(pipes.data(),1048576,_O_BINARY)==-1)
#else
		if(::pipe(pipes.data())==-1)
#endif
			throw std::system_error(errno,std::generic_category());
	}
	posix_pipe(posix_pipe&& other) noexcept:pipes(other.pipes)
	{
		other.pipes.fill(-1);
	}
	posix_pipe& operator=(posix_pipe && other) noexcept
	{
		if(pipes.data()!=other.pipes.data())
		{
			close_impl();
			pipes = other.pipes;
			other.pipes.fill(-1);
		}
		return *this;
	}
	template<std::size_t om>
	posix_pipe(open::interface_t<om>):posix_pipe()
	{
		if constexpr ((!om)||(om&~(open::in.value|open::out.value)))
			throw std::runtime_error("unknown posix pipe flags");
		if constexpr (!(om&~open::in.value)&&(om&~open::out.value))
		{
			close(pipes.front());
			pipes.front()=-1;
		}
		if constexpr ((om&~open::in.value)&&!(om&~open::out.value))
		{
			close(pipes.back());			
			pipes.back()=-1;
		}
	}
	posix_pipe(posix_pipe const& b)
	{
		std::size_t i(0);
		try
		{
			for(;i!=b.pipes.size();++i)
				if(b.pipes[i]==-1)
					pipes[i]=-1;
				else
				{
					auto newfd(dup(b.pipes[i]));
					if(newfd<0)
						throw std::system_error(errno,std::generic_category());
					pipes[i]=newfd;
				}
		}
		catch(...)
		{
			for(;i;--i)
				if(pipes[i-1]!=-1)
					close(pipes[i-1]);
			throw;
		}
	}
	posix_pipe& operator=(posix_pipe const& m)
	{
		std::size_t i(0);
		native_handle_type new_hds;
		try
		{
			for(;i!=m.pipes.size();++i)
				if(m.pipes[i]==-1)
					new_hds[i]=-1;
				else
				{
					auto newfd(dup(m.pipes[i]));
					if(newfd==-1)
						throw std::system_error(errno,std::generic_category());
					new_hds[i]=newfd;
				}
		}
		catch(...)
		{
			for(;i;--i)
				if(new_hds[i-1]!=-1)
					close(new_hds[i-1]);
			throw;
		}
		for(auto const & e : pipes)
			if(e!=-1)
				close(e);
		pipes=new_hds;
		return *this;
	}
	~posix_pipe()
	{
		close_impl();
	}
	auto& native_handle()
	{
		return pipes;
	}
	void flush()
	{
	}
	void close_in()
	{
		if(pipes.front()!=-1)
		{
			close(pipes.front());
			pipes.front() = -1;
		}
	}
	void close_out()
	{
		if(pipes.back()!=-1)
		{
			close(pipes.back());
			pipes.back() = -1;
		}
	}
	template<typename ContiguousIterator>
	ContiguousIterator read(ContiguousIterator begin,ContiguousIterator end)
	{
		auto read_bytes(::read(pipes.front(),std::addressof(*begin),(end-begin)*sizeof(*begin)));
		if(read_bytes==-1)
			throw std::system_error(errno,std::generic_category());
		return begin+(read_bytes/sizeof(*begin));
	}
	template<typename ContiguousIterator>
	ContiguousIterator write(ContiguousIterator begin,ContiguousIterator end)
	{
		auto write_bytes(::write(pipes.back(),std::addressof(*begin),(end-begin)*sizeof(*begin)));
		if(write_bytes==-1)
			throw std::system_error(errno,std::generic_category());
		return begin+(write_bytes/sizeof(*begin));
	}
};
}