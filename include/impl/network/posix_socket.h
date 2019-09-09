#pragma once
#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>
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
	socket(native_handle_t,native_handle_type skt):handle(skt)
	{
		if(handle==-1)
			throw std::system_error(errno,std::generic_category());
	}
	template<typename ...Args>
	socket(native_interface_t,Args&& ...args):socket(fast_io::native_handle,::socket(std::forward<Args>(args)...)){}

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
	template<typename ContiguousIterator>
	ContiguousIterator read(ContiguousIterator begin,ContiguousIterator end)
	{
		auto read_bytes(::recv(handle,std::addressof(*begin),(end-begin)*sizeof(*begin),0));
		if(read_bytes==-1)
			throw std::system_error(errno,std::generic_category());
		return begin+(read_bytes/sizeof(*begin));
	}
	template<typename ContiguousIterator>
	ContiguousIterator write(ContiguousIterator begin,ContiguousIterator end)
	{
		auto write_bytes(::send(handle,std::addressof(*begin),(end-begin)*sizeof(*begin),0));
		if(write_bytes==-1)
			throw std::system_error(errno,std::generic_category());
		return begin+(write_bytes/sizeof(*begin));
	}
	void flush(){}
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
		sockaddr_in servaddr{static_cast<std::uint16_t>(fm),htons(add.port())};
		if(::inet_pton(static_cast<int>(fm),addr.data(),std::addressof(servaddr.sin_addr))==-1)
			throw std::system_error(errno,std::generic_category());
		if(::connect(soc.native_handle(),static_cast<sockaddr const*>(static_cast<void const*>(std::addressof(servaddr))),sizeof(servaddr))==-1)
			throw std::system_error(errno,std::generic_category());
	}
	auto& handle()
	{
		return soc;
	}
	template<typename ...Args>
	auto read(Args&& ...args)
	{
		return soc.read(std::forward<Args>(args)...);
	}
	template<typename ...Args>
	void write(Args&& ...args)
	{
		soc.write(std::forward<Args>(args)...);
	}
	void flush()
	{
		soc.flush();
	}
};

class server_socket
{
	socket soc;
	sockaddr_in add;
public:
	using native_handle_type = int;
	using char_type = char;
	server_socket(socket& listener_socket):soc(fast_io::native_handle,-1)
	{
		socklen_t size(sizeof(add));
		auto ret(::accept(listener_socket.native_handle(),static_cast<sockaddr*>(static_cast<void*>(std::addressof(add))),std::addressof(size)));
		if(ret==-1)
			throw std::system_error(errno,std::generic_category());
		soc=socket(fast_io::native_handle,ret);
	}
	auto& handle()
	{
		return soc;
	}
	template<typename ...Args>
	auto read(Args&& ...args)
	{
		return soc.read(std::forward<Args>(args)...);
	}
	template<typename ...Args>
	void write(Args&& ...args)
	{
		soc.write(std::forward<Args>(args)...);
	}
	void flush()
	{
		soc.flush();
	}
	auto& native_address() const
	{
		return add;
	}
};

class server
{
	socket soc;
public:
	template<typename ...Args>
	server(sock::family const & fm,address const& add,Args&& ...args):soc(fm,std::forward<Args>(args)...)
	{
		sockaddr_in servaddr{static_cast<sa_family_t>(fm),htons(add.port()),{},{}};
		if(::inet_pton(static_cast<int>(fm),add.addr().data(),std::addressof(servaddr.sin_addr))==-1)
			throw std::system_error(errno,std::generic_category());
		if(::bind(soc.native_handle(),static_cast<sockaddr const*>(static_cast<void const*>(std::addressof(servaddr))),sizeof(servaddr))==-1)
			throw std::system_error(errno,std::generic_category());
		if(::listen(soc.native_handle(),10)==-1)
			throw std::system_error(errno,std::generic_category());
	}
	auto& handle()
	{
		return soc;
	}
	auto accept()
	{
		return server_socket(soc);
	}
};

}