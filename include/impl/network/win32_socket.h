#pragma once

namespace fast_io
{
namespace details
{
inline auto const ws2_32_dll(LoadLibraryW(L"ws2_32.dll"));
inline auto socket(reinterpret_cast<decltype(::socket)*>(reinterpret_cast<void(*)()>(GetProcAddress(ws2_32_dll,"socket"))));
inline auto WSAGetLastError(reinterpret_cast<decltype(::WSAGetLastError)*>(reinterpret_cast<void(*)()>(GetProcAddress(ws2_32_dll,"WSAGetLastError"))));
inline auto connect(reinterpret_cast<decltype(::connect)*>(reinterpret_cast<void(*)()>(GetProcAddress(ws2_32_dll,"connect"))));
inline auto closesocket(reinterpret_cast<decltype(::closesocket)*>(reinterpret_cast<void(*)()>(GetProcAddress(ws2_32_dll,"closesocket"))));
inline auto send(reinterpret_cast<decltype(::send)*>(reinterpret_cast<void(*)()>(GetProcAddress(ws2_32_dll,"send"))));
inline auto recv(reinterpret_cast<decltype(::recv)*>(reinterpret_cast<void(*)()>(GetProcAddress(ws2_32_dll,"recv"))));
inline auto htons(reinterpret_cast<decltype(::htons)*>(reinterpret_cast<void(*)()>(GetProcAddress(ws2_32_dll,"htons"))));
inline auto InetPtonW(reinterpret_cast<decltype(::InetPtonW)*>(reinterpret_cast<void(*)()>(GetProcAddress(ws2_32_dll,"InetPtonW"))));
inline auto bind(reinterpret_cast<decltype(::bind)*>(reinterpret_cast<void(*)()>(GetProcAddress(ws2_32_dll,"bind"))));
inline auto listen(reinterpret_cast<decltype(::listen)*>(reinterpret_cast<void(*)()>(GetProcAddress(ws2_32_dll,"listen"))));
inline auto accept(reinterpret_cast<decltype(::accept)*>(reinterpret_cast<void(*)()>(GetProcAddress(ws2_32_dll,"accept"))));
class win32_startup
{
public:
	win32_startup()
	{
		auto WSAStartup(reinterpret_cast<decltype(::WSAStartup)*>(reinterpret_cast<void(*)()>(GetProcAddress(ws2_32_dll,"WSAStartup"))));
		WSADATA data;
		if(auto error_code=WSAStartup(2<<8|2,std::addressof(data)))
			throw std::system_error(error_code,std::generic_category());
	}
	win32_startup(win32_startup const&) = delete;
	win32_startup& operator=(win32_startup const&) = delete;
	~win32_startup()
	{
		auto WSACleanup(reinterpret_cast<decltype(::WSACleanup)*>(reinterpret_cast<void(*)()>(GetProcAddress(ws2_32_dll,"WSACleanup"))));
		WSACleanup();
	}
};

inline win32_startup const startup;
}
class acceptor;
class socket
{
	SOCKET handle;
	void close_impl()
	{
		if(handle!=INVALID_SOCKET)
			details::closesocket(handle);
	}
	friend class acceptor;
	socket()=default;
public:
	template<typename ...Args>
	socket(native_interface_t,Args&& ...args):handle(details::socket(std::forward<Args>(args)...))
	{
		if(handle==INVALID_SOCKET)
			throw std::system_error(details::WSAGetLastError(),std::generic_category());
	}
	socket(sock::family const & family,sock::type const &type,sock::protocal const &protocal = sock::protocal::none):socket(native_interface,static_cast<ADDRESS_FAMILY>(family),static_cast<int>(type),static_cast<int>(protocal)){}
	auto native_handle() const {return handle;}
	socket(socket const&) = delete;
	socket& operator=(socket const&) = delete;
	socket(socket && soc) noexcept:handle(soc.handle)
	{
		soc.handle = INVALID_SOCKET;
	}
	socket& operator=(socket && soc) noexcept
	{
		if(soc.handle!=handle)
		{
			close_impl();
			handle = soc.handle;
			soc.handle = INVALID_SOCKET;
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
		auto read_bytes(details::recv(handle,std::addressof(*begin),static_cast<int>((end-begin)*sizeof(*begin)),0));
		if(read_bytes==SOCKET_ERROR)
			throw std::system_error(details::WSAGetLastError(),std::generic_category());
		return begin+(read_bytes/sizeof(*begin));
	}
	template<typename ContiguousIterator>
	ContiguousIterator write(ContiguousIterator begin,ContiguousIterator end)
	{
		auto write_bytes(details::send(handle,std::addressof(*begin),static_cast<int>((end-begin)*sizeof(*begin)),0));
		if(write_bytes==SOCKET_ERROR)
			throw std::system_error(details::WSAGetLastError(),std::generic_category());
		return begin+(write_bytes/sizeof(*begin));
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

class client
{
	socket soc;
public:
	using char_type = char;
	template<typename ...Args>
	client(sock::family const & fm,address const& add,Args&& ...args):soc(fm,std::forward<Args>(args)...)
	{
		auto addr(fast_io::utf8_to_ucs<std::wstring>(add.addr()));
		sockaddr_in servaddr{static_cast<ADDRESS_FAMILY>(fm),details::htons(add.port()),{},{}};
		if(details::InetPtonW(static_cast<int>(fm),addr.data(),std::addressof(servaddr.sin_addr))==-1)
			throw std::system_error(details::WSAGetLastError(),std::generic_category());
		if(details::connect(soc.native_handle(),static_cast<sockaddr const*>(static_cast<void const*>(std::addressof(servaddr))),sizeof(servaddr))==-1)
			throw std::system_error(details::WSAGetLastError(),std::generic_category());
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

class server
{
	socket soc;
public:
	template<typename ...Args>
	server(sock::family const & fm,address const& add,Args&& ...args):soc(fm,std::forward<Args>(args)...)
	{
		sockaddr_in servaddr{static_cast<ADDRESS_FAMILY>(fm),details::htons(add.port()),{},{}};
		auto addr(fast_io::utf8_to_ucs<std::wstring>(add.addr()));
		if(details::InetPtonW(static_cast<int>(fm),addr.data(),std::addressof(servaddr.sin_addr))==-1)
			throw std::system_error(details::WSAGetLastError(),std::generic_category());
		if(details::bind(soc.native_handle(),static_cast<sockaddr const*>(static_cast<void const*>(std::addressof(servaddr))),sizeof(servaddr))==SOCKET_ERROR)
			throw std::system_error(details::WSAGetLastError(),std::generic_category());
		if(details::listen(soc.native_handle(),10)==SOCKET_ERROR)
			throw std::system_error(details::WSAGetLastError(),std::generic_category());			
	}
	auto& handle()
	{
		return soc;
	}
};

class acceptor
{
	socket soc;
	sockaddr_in add;
public:
	using native_handle_type = SOCKET;
	using char_type = char;
	acceptor(server& listener_socket)
	{
		int size(sizeof(add));
		auto ret(details::accept(listener_socket.handle().native_handle(),static_cast<sockaddr*>(static_cast<void*>(std::addressof(add))),std::addressof(size)));
		if(ret==INVALID_SOCKET)
			throw std::system_error(details::WSAGetLastError(),std::generic_category());
		soc.handle=ret;
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

}