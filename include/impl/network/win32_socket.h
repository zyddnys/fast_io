#pragma once

namespace fast_io::sock::details
{

inline auto load_ws2_32()
{
	auto ws2_32_dll(LoadLibraryW(L"ws2_32.dll"));
	if(ws2_32_dll==nullptr)
		throw win32_error();
	return ws2_32_dll;
}

inline auto const ws2_32_dll{load_ws2_32()};

template<typename prototype>
inline auto get_proc_address(char const* proc)
{
	auto address(GetProcAddress(ws2_32_dll,proc));
	if(address==nullptr)
		throw win32_error();
	return reinterpret_cast<prototype>(reinterpret_cast<void(*)()>(address));
}

inline auto get_last_error(get_proc_address<decltype(::WSAGetLastError)*>("WSAGetLastError"));

template<typename prototype,typename ...Args>
inline auto call_win32_ws2_32(char const *name,Args&& ...args)
{
	auto ret(get_proc_address<prototype>(name)(std::forward<Args>(args)...));
	if(ret==SOCKET_ERROR)
		throw std::system_error(get_last_error(),std::system_category());
	return ret;
}

template<typename prototype,typename ...Args>
inline auto call_win32_ws2_32_invalid_socket(char const *name,Args&& ...args)
{
	auto ret(get_proc_address<prototype>(name)(std::forward<Args>(args)...));
	if(ret==INVALID_SOCKET)
		throw std::system_error(get_last_error(),std::system_category());
	return ret;
}

template<typename prototype,typename ...Args>
inline auto call_win32_ws2_32_minus_one(char const *name,Args&& ...args)
{
	auto ret(get_proc_address<prototype>(name)(std::forward<Args>(args)...));
	if(ret==-1)
		throw std::system_error(get_last_error(),std::system_category());
	return ret;
}

template<typename ...Args>
inline auto socket(Args&& ...args)
{
	return call_win32_ws2_32_invalid_socket<decltype(::socket)*>("socket",std::forward<Args>(args)...);
}

template<typename T>
inline auto accept(SOCKET sck,T& sock_address,socklen_t& storage_size)
{
	return call_win32_ws2_32_invalid_socket<decltype(::accept)*>("accept",sck,static_cast<sockaddr*>(static_cast<void*>(std::addressof(sock_address))),std::addressof(storage_size));
}

template<typename sock_type,typename T>
inline auto connect(SOCKET sck,T& sock_address)
{
	return call_win32_ws2_32_minus_one<decltype(::connect)*>("connect",sck,static_cast<sockaddr*>(static_cast<void*>(std::addressof(sock_address))),sizeof(T));
}

template<typename ...Args>
inline auto send(Args&& ...args)
{
	return call_win32_ws2_32<decltype(::send)*>("send",std::forward<Args>(args)...);
}
template<typename ...Args>
inline auto recv(Args&& ...args)
{
	return call_win32_ws2_32<decltype(::recv)*>("recv",std::forward<Args>(args)...);
}

template<typename ...Args>
inline auto closesocket(Args&& ...args)
{
	return call_win32_ws2_32<decltype(::closesocket)*>("closesocket",std::forward<Args>(args)...);
}

template<typename sock_type,typename T>
inline auto bind(SOCKET sck,T& sock_address)
{
	return call_win32_ws2_32_minus_one<decltype(::bind)*>("bind",sck,static_cast<sockaddr*>(static_cast<void*>(std::addressof(sock_address))),sizeof(sock_type));
}

template<typename ...Args>
inline auto listen(Args&& ...args)
{
	return call_win32_ws2_32<decltype(::listen)*>("listen",std::forward<Args>(args)...);
}


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

using address_family =
#ifdef _MSC_VER
ADDRESS_FAMILY;
#else
std::int16_t;
#endif


using socket_type = SOCKET;
auto constexpr invalid_socket(INVALID_SOCKET);
}