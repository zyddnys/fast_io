#pragma once

namespace fast_io::sock::details
{

template<typename Func,typename ...Args>
inline auto call_posix(Func&& func,Args&& ...args)
{
	auto ret(func(std::forward<Args>(args)...));
	if(ret==-1)
		throw std::system_error(errno,std::generic_category());
	return ret;
}

template<typename ...Args>
inline auto socket(Args&& ...args)
{
	return call_posix(::socket,std::forward<Args>(args)...);
}

template<typename T>
inline auto accept(int sck,T& sock_address,socklen_t& storage_size)
{
	return call_posix(::accept,sck,static_cast<sockaddr*>(static_cast<void*>(std::addressof(sock_address))),std::addressof(storage_size));
}

template<typename sock_type,typename T>
inline auto connect(int sck,T& sock_address)
{
	return call_posix(::connect,sck,static_cast<sockaddr*>(static_cast<void*>(std::addressof(sock_address))),sizeof(sock_type));
}

template<typename ...Args>
inline auto send(Args&& ...args)
{
	return call_posix(::send,std::forward<Args>(args)...);
}
template<typename ...Args>
inline auto recv(Args&& ...args)
{
	return call_posix(::recv,std::forward<Args>(args)...);
}

template<typename ...Args>
inline auto closesocket(Args&& ...args)
{
	return call_posix(::close,std::forward<Args>(args)...);
}

template<typename sock_type,typename T>
inline auto bind(int sck,T& sock_address)
{
	return call_posix(::bind,sck,static_cast<sockaddr*>(static_cast<void*>(std::addressof(sock_address))),sizeof(sock_type));
}

template<typename ...Args>
inline auto bind(Args&& ...args)
{
	return call_posix(::bind,std::forward<Args>(args)...);
}

template<typename ...Args>
inline auto listen(Args&& ...args)
{
	return call_posix(::listen,std::forward<Args>(args)...);
}


using address_family = sa_family_t;
using socket_type = int;
auto constexpr invalid_socket(-1);
}