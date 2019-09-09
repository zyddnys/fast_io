#pragma once
#include<unistd.h>
#include<fcntl.h>
namespace fast_io
{
class socket
{
	int handle;
	void close_impl()
	{
		if(handle!=-1)
			::close(handle);
	}
public:
	using native_handle_type = int;
	template<typename ...Args>
	socket(native_interface_t,Args&& ...args):handle(::socket(std::forward<Args>(args)...))
	{
		if(handle==-1)
			throw std::system_error(errno,std::generic_category());
	}
	socket(sock::family const & family,sock::type const &type,sock::protocal const &protocal = sock::protocal::none)
			:socket(native_interface,static_cast<int>(family),static_cast<int>(type),static_cast<int>(protocal)){}
	auto native_handle() const {return handle;}
	socket(socket const&) = delete;
	socket& operator=(socket const&) = delete;
	socket(socket && soc) noexcept:handle(soc.handle)
	{
		soc.handle = -1;
	}
	socket& operator=(socket && soc) noexcept
	{
		if(soc.handle!=handle)
		{
			close_impl();
			handle = soc.handle;
			soc.handle = -1;
		}
		return *this;
	}
	void swap(socket& b) noexcept
	{
		using std::swap;
		swap(handle,b.handle);
	}
	~socket()
	{
		close_impl();
	}
};

inline constexpr void swap(socket& a,socket &b) noexcept
{
	swap(a,b);
}

class client
{
	socket soc;
public:
	using char_type = char;
	template<typename ...Args>
	client(sock::family const & fm,address const& add,Args&& ...args):soc(fm,std::forward<Args>(args)...)
	{
		auto addr(add.addr());
		sockaddr saddr{static_cast<std::int16_t>(fm),htons(add.port())};
		std::uninitialized_copy(addr.cbegin(),addr.cend(),static_cast<in_addr*>(static_cast<void*>(std::addressof(saddr.sa_data))));
		if(::connect(soc.native_handle(),std::addressof(saddr),addr.size())==-1)
			throw std::system_error(errno,std::generic_category());
	}
	auto& handle()
	{
		return soc;
	}
};

}