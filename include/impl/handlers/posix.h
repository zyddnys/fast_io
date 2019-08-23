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

template<open_mode_interface inf>
struct posix_file_open_mode
{
	static auto constexpr mode = []()
	{
		std::tuple<int,int> tp;

		return md;
	}();
};
}

class posix_file
{
	int fd;
	void close_impl() noexcept
	{
		if(fd!=-1)
			close(fd);
	}

public:
	using char_type = char;
	using native_handle_type = int;
	template<typename T>
	posix_file(std::string_view file,open_interface_t<T>):fd(std::apply([&file](auto mode,auto flag)
	{
		return ::open(file.data(),mode,flag);
	})(posix_file_open_flags<T>::mode))//open(file.data(),posix_file_open_flags<decltype(interface)>::mode,posix_file_open_flags<decltype(interface)>::flag))
	{
		if(fd==-1)
			throw std::system_error(errno,std::generic_category());
	}
	template<typename ...Args>
	posix_file(fast_io::native_interface_t,Args&& ...args):fd(open(std::forward<Args>(args)...))
	{
		if(fd==-1)
			throw std::system_error(errno,std::generic_category());
	}
	posix_file(const posix_file&)=delete;
	posix_file& operator=(const posix_file&)=delete;
	posix_file(posix_file&& b) noexcept : fd(b.fd)
	{
		b.fd = -1;
	}
	posix_file& operator=(posix_file&& b) noexcept
	{
		if(&b!=this)
		{
			close_impl();
			fd=b.fd;
			b.fd = -1;
		}
		return *this;
	}
	native_handle_type native_handle() const
	{
		return fd;
	}
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
		if(fsync(fd)==-1)
			throw std::system_error(errno,std::system_category());
	}
	~posix_file()
	{
		close_impl();
	}
};
}