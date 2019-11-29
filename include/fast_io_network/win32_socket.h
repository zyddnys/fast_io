#pragma once
#include"mswsock.h"

namespace fast_io::sock::details
{
namespace
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
	return bit_cast<prototype>(address);
}

inline auto get_last_error(get_proc_address<decltype(::WSAGetLastError)*>("WSAGetLastError"));

template<typename prototype,typename ...Args>
inline auto call_win32_ws2_32(char const *name,Args&& ...args)
{
	auto ret(get_proc_address<prototype>(name)(std::forward<Args>(args)...));
	if(ret==SOCKET_ERROR)
		throw win32_error(get_last_error());
	return ret;
}

template<typename prototype,typename ...Args>
inline auto call_win32_ws2_32_invalid_socket(char const *name,Args&& ...args)
{
	auto ret(get_proc_address<prototype>(name)(std::forward<Args>(args)...));
	if(ret==INVALID_SOCKET)
		throw win32_error(get_last_error());
	return ret;
}

template<typename prototype,typename ...Args>
inline auto call_win32_ws2_32_minus_one(char const *name,Args&& ...args)
{
	auto ret(get_proc_address<prototype>(name)(std::forward<Args>(args)...));
	if(ret==-1)
		throw win32_error(get_last_error());
	return ret;
}

template<typename prototype,typename ...Args>
inline auto call_win32_ws2_32_nullptr(char const *name,Args&& ...args)
{
	auto ret(get_proc_address<prototype>(name)(std::forward<Args>(args)...));
	if(ret==nullptr)
		throw win32_error(get_last_error());
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
	return call_win32_ws2_32_invalid_socket<decltype(::accept)*>("accept",sck,reinterpret_cast<sockaddr*>(std::addressof(sock_address)),std::addressof(storage_size));
}

template<typename T,std::unsigned_integral sock_type_size>
inline auto connect(SOCKET sck,T& sock_address,sock_type_size size)
{
	return call_win32_ws2_32_minus_one<decltype(::connect)*>("connect",sck,reinterpret_cast<sockaddr*>(std::addressof(sock_address)),static_cast<int>(size));
}

template<typename mem_address,typename ...Args>
inline auto send(SOCKET sock,mem_address const* add,Args&& ...args)
{
	return call_win32_ws2_32<decltype(::send)*>("send",sock,static_cast<char const*>(static_cast<void const*>(add)),std::forward<Args>(args)...);
}
template<typename mem_address,typename ...Args>
inline auto recv(SOCKET sock,mem_address* add,Args&& ...args)
{
	return call_win32_ws2_32<decltype(::recv)*>("recv",sock,static_cast<char*>(static_cast<void*>(add)),std::forward<Args>(args)...);
}

template<typename ...Args>
inline auto closesocket(Args&& ...args)
{
	return call_win32_ws2_32<decltype(::closesocket)*>("closesocket",std::forward<Args>(args)...);
}

template<typename T,std::unsigned_integral sock_type_size>
inline auto bind(SOCKET sck,T& sock_address,sock_type_size size)
{
	return call_win32_ws2_32_minus_one<decltype(::bind)*>("bind",sck,reinterpret_cast<sockaddr*>(std::addressof(sock_address)),static_cast<int>(size));
}

template<typename ...Args>
inline auto listen(Args&& ...args)
{
	return call_win32_ws2_32<decltype(::listen)*>("listen",std::forward<Args>(args)...);
}

template<typename ...Args>
inline void freeaddrinfo(Args&& ...args)
{
	get_proc_address<decltype(::freeaddrinfo)*>("freeaddrinfo")(std::forward<Args>(args)...);
}

class win32_startup
{
public:
	win32_startup()
	{
		auto WSAStartup(reinterpret_cast<decltype(::WSAStartup)*>(reinterpret_cast<void(*)()>(GetProcAddress(ws2_32_dll,"WSAStartup"))));
		WSADATA data;
		if(auto error_code=WSAStartup(2<<8|2,std::addressof(data)))
			throw win32_error(error_code);
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
inline constexpr auto invalid_socket(INVALID_SOCKET);

}
}


namespace fast_io
{

class gai_exception:public std::exception
{
	int ec;
public:
	explicit gai_exception(int errorc):ec(errorc){}
	auto get() const noexcept
	{
		return ec;
	}
	char const* what() const noexcept
	{
		switch(ec)
		{
		case EAI_AGAIN:
			return "A temporary failure in name resolution occurred.";
		case EAI_BADFLAGS:
			return "An invalid value was provided for the ai_flags member of the pHints parameter.";
		case EAI_FAIL:
			return "A nonrecoverable failure in name resolution occurred.";
		case EAI_FAMILY:
			return "The ai_family member of the pHints parameter is not supported.";
		case EAI_MEMORY:
			return "A memory allocation failure occurred.";
		case EAI_NONAME:
			return "The name does not resolve for the supplied parameters or the pNodeName and pServiceName parameters were not provided.";
		case EAI_SERVICE:
			return "The pServiceName parameter is not supported for the specified ai_socktype member of the pHints parameter.";
		case EAI_SOCKTYPE:
			return "The ai_socktype member of the pHints parameter is not supported.";
		default:
			return "unknown";
		}
	}
};

namespace sock::details
{
template<typename ...Args>
inline void getaddrinfo(Args&& ...args)
{
	auto ec(get_proc_address<decltype(::getaddrinfo)*>("getaddrinfo")(std::forward<Args>(args)...));
	if(ec)
		throw gai_exception(ec);
}
}
}

namespace fast_io
{
//zero copy IO for win32
namespace details::win32
{
template<zero_copy_output_stream output,zero_copy_input_stream input>
inline std::size_t zero_copy_transmit_once(output& outp,input& inp,std::size_t bytes)
{
	if(!::TransmitFile(zero_copy_out_handle(outp),zero_copy_in_handle(inp),bytes,0,nullptr,nullptr,TF_USE_DEFAULT_WORKER))
		throw std::system_error(errno,std::generic_category());
	return bytes;
}
}

template<zero_copy_output_stream output,zero_copy_input_stream input>
inline std::size_t zero_copy_transmit(output& outp,input& inp,std::size_t bytes)
{
	std::size_t constexpr maximum_transmit_bytes(2147483646);
	std::size_t transmitted(0);
	for(;bytes;)
	{
		std::size_t should_transfer(maximum_transmit_bytes);
		if(bytes<should_transfer)
			should_transfer=bytes;
		std::size_t transferred_this_round(details::win32::zero_copy_transmit_once(outp,inp,should_transfer));
		transmitted+=transferred_this_round;
		if(transferred_this_round!=should_transfer)
			return transmitted;
		bytes-=transferred_this_round;
	}
	return transmitted;
	
}
template<zero_copy_output_stream output,zero_copy_input_stream input>
inline std::size_t zero_copy_transmit(output& outp,input& inp)
{
	constexpr std::size_t maximum_transmit_bytes(2147483646);
	for(std::size_t transmitted(0);;)
	{
		std::size_t transferred_this_round(details::win32::zero_copy_transmit_once(outp,inp,maximum_transmit_bytes));
		transmitted+=transferred_this_round;
		if(transferred_this_round!=maximum_transmit_bytes)
			return transmitted;
	}
}
}