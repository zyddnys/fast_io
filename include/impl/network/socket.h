#pragma once

#if defined(__WINNT__) || defined(_MSC_VER)
#include "win32_socket.h"
#else
#include "posix_socket.h"
#endif
//#include<bit>

namespace fast_io
{

inline constexpr auto ipv4_address(std::string_view str)
{
	std::array<std::uint8_t,4> value{};
	if(str.empty())
		return value;
	istring_view istrbuf(str);
	for(auto & e : value)
		scan(istrbuf,e);
	return value;
}

inline auto ipv6_address(std::string_view str)
{
	std::array<std::uint16_t,8> value{};
	if(str.empty())
		return value;
	ostring ostr;
	std::size_t prefix_zero(0);
	for(auto it(str.cbegin());it!=str.cend();++it)
	{
		std::uint8_t e(*it);
		if(e==':')
		{
			if(it==str.cbegin())
				ostr.put('0');
			++prefix_zero;
			ostr.put(' ');
			if(it+1==str.cend())
				ostr.put('0');
			else if(it[1]==':')
			{
				ostr.put('0');
				auto j(it+1);
				for(;j!=str.cend();++j)
					if(*j==':')
						++prefix_zero;
				for(;prefix_zero<7;++prefix_zero)
				{
					ostr.put(' ');
					ostr.put('0');
				}
			}
		}
		else if(e-'0'<10||e-'a'<6||e-'A'<6)
			ostr.put(e);
		else
			break;
	}
	istring_view istrbuf(ostr.str());
	for(auto & e : value)
		scan(istrbuf,fast_io::hex(e));
	return value;
}

namespace sock::details
{

template<std::unsigned_integral U>
inline constexpr void in_place_big_endian(U& u)
{
//	if constexpr (std::endian::native==std::endian::little)			//pretend the platform is little endian before c++20
//	{
		auto &e(reinterpret_cast<std::array<std::uint8_t,sizeof(U)>&>(u));
		std::reverse(e.begin(),e.end());
//	}
}

inline void set_ipv4_storage(sockaddr_storage& storage,address const& add)
{
	auto& sockaddin(reinterpret_cast<sockaddr_in&>(storage));
//	sockaddin={};
	sockaddin.sin_family=static_cast<address_family>(fast_io::sock::family::ipv4);
	in_place_big_endian(sockaddin.sin_port=add.port());
	if(!add.addr().empty())
		reinterpret_cast<std::array<std::uint8_t,4>&>(sockaddin.sin_addr)=ipv4_address(add.addr());
}

inline void set_ipv6_storage(sockaddr_storage& storage,address const& add)
{
	auto& sockaddin(reinterpret_cast<sockaddr_in6&>(storage));
//	sockaddin={};
	sockaddin.sin6_family=static_cast<address_family>(fast_io::sock::family::ipv6);
	in_place_big_endian(sockaddin.sin6_port=add.port());
	if(!add.addr().empty())
		reinterpret_cast<std::array<std::uint16_t,8>&>(sockaddin.sin6_addr)=ipv6_address(add.addr());
}
}

class acceptor;
class socket
{
	sock::details::socket_type handle;
	void close_impl()
	try
	{
		if(handle!=sock::details::invalid_socket)
			sock::details::closesocket(handle);
	}
	catch(...)
	{}
	friend class acceptor;
	socket()=default;
public:
	template<typename ...Args>
	socket(native_interface_t,Args&& ...args):handle(sock::details::socket(std::forward<Args>(args)...)){}
	socket(sock::family const & family,sock::type const &type,sock::protocal const &protocal = sock::protocal::none):
		handle(sock::details::socket(static_cast<sock::details::address_family>(family),static_cast<int>(type),static_cast<int>(protocal))){}
	auto native_handle() const {return handle;}
	socket(socket const&) = delete;
	socket& operator=(socket const&) = delete;
	socket(socket && soc) noexcept:handle(soc.handle)
	{
		soc.handle = sock::details::invalid_socket;
	}
	socket& operator=(socket && soc) noexcept
	{
		if(soc.handle!=handle)
		{
			close_impl();
			handle = soc.handle;
			soc.handle = sock::details::invalid_socket;
		}
		return *this;
	}
	void swap(socket& b) noexcept
	{
		using std::swap;
		swap(handle,b.handle);
	}
	template<typename ContiguousIterator>
	ContiguousIterator read(ContiguousIterator begin,ContiguousIterator end)
	{
		return begin+((sock::details::recv(handle,std::addressof(*begin),static_cast<int>((end-begin)*sizeof(*begin)),0))/sizeof(*begin));
	}
	template<typename ContiguousIterator>
	ContiguousIterator write(ContiguousIterator begin,ContiguousIterator end)
	{
		return begin+(sock::details::send(handle,std::addressof(*begin),static_cast<int>((end-begin)*sizeof(*begin)),0)/sizeof(*begin));
	}
	~socket()
	{
		close_impl();
	}
	void flush()
	{
	}
};

inline constexpr void swap(socket& a,socket &b) noexcept
{
	swap(a,b);
}

class address_info
{
	sockaddr_storage storage={};
	socklen_t size=0;
public:
	auto& native_storage() {return storage;}
	auto& native_storage_size() {return size;}
	bool v4()
	{
		return size == sizeof(sockaddr_in);
	}
	bool v6()
	{
		return size == sizeof(sockaddr_in6);
	}
	bool unknown()
	{
		return !v4()&&!v6();
	}
/*	std::string address()
	{
		
	}*/
	
};

class client
{
	socket soc;
	address_info cinfo;
public:
	using char_type = char;
	template<typename ...Args>
	client(sock::family const & fm,address const& add,Args&& ...args):soc(fm,std::forward<Args>(args)...)
	{
		if(fm==sock::family::ipv6)
		{
			sock::details::set_ipv6_storage(cinfo.native_storage(),add);
			sock::details::connect<sockaddr_in6>(soc.native_handle(),cinfo.native_storage());
		}
		else if(fm==sock::family::ipv4)
		{
			sock::details::set_ipv4_storage(cinfo.native_storage(),add);
			sock::details::connect<sockaddr_in>(soc.native_handle(),cinfo.native_storage());
		}
		else
			throw std::runtime_error("currently not supported protocals");
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
	auto& info() const
	{
		return cinfo;
	}
};

class server
{
	sock::family fml;
	socket soc;
public:
	template<typename ...Args>
	server(sock::family const & fm,address const& add,Args&& ...args):fml(fm),soc(fm,std::forward<Args>(args)...)
	{
		sockaddr_storage storage{};
		if(fm==sock::family::ipv6)
		{
			sock::details::set_ipv6_storage(storage,add);
			sock::details::bind<sockaddr_in6>(soc.native_handle(),storage);
		}
		else if(fm==sock::family::ipv4)
		{
			sock::details::set_ipv4_storage(storage,add);
			sock::details::bind<sockaddr_in>(soc.native_handle(),storage);
		}
		else
			throw std::runtime_error("currently not supported protocals");
		sock::details::listen(soc.native_handle(),10);
	}
	auto& handle()
	{
		return soc;
	}
	auto& family()
	{
		return fml;
	}
};

class acceptor
{
	socket soc;
	address_info add;
public:
	using native_handle_type = sock::details::socket_type;
	using char_type = char;
	acceptor(server& listener_socket)
	{
		soc.handle=sock::details::accept(listener_socket.handle().native_handle(),add.native_storage(),add.native_storage_size());
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
	auto& info() const
	{
		return add;
	}
};
}