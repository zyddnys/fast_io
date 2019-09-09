#pragma once
#include <winsock2.h>
#include"../handlers/win32_error.h"

namespace fast_io
{
namespace details
{
inline auto const ws2_32_dll(LoadLibraryW(L"ws2_32.dll"));
inline auto socket(reinterpret_cast<decltype(::socket)*>(reinterpret_cast<void(*)()>(GetProcAddress(ws2_32_dll,"socket"))));
inline auto WSAGetLastError(reinterpret_cast<decltype(::WSAGetLastError)*>(reinterpret_cast<void(*)()>(GetProcAddress(ws2_32_dll,"WSAGetLastError"))));

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

inline win32_startup startup;
}

class socket
{
	SOCKET handle;
public:
	using native_handle_type = SOCKET;
	template<typename ...Args>
	socket(native_interface_t,Args&& ...args):handle(details::socket(std::forward<Args>(args)...))
	{
		if(handle==INVALID_SOCKET)
			throw std::system_error(details::WSAGetLastError(),std::generic_category());
	}
	socket(sock::family const & family,sock::type const &type):socket(native_interface,static_cast<int>(family),static_cast<int>(type),0){}
	auto native_handle() const {return handle;}
};

class client
{
public:
	using char_type = char;

};


}