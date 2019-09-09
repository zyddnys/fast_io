#pragma once

#ifdef __WINNT__
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <sys/socket.h>
#include<netinet/in.h>
#endif

namespace fast_io
{
namespace sock
{
enum class family
{
unspec = AF_UNSPEC,
unix = AF_UNIX,
ipx = AF_IPX,
apple_talk = AF_APPLETALK, 
ipv4 = AF_INET,
ipv6 = AF_INET6,
/*icmp = PF_ICMP,
igmp = PF_IGMP,
bluetooth_rfcomm = RFCOMM,
icmpv6 = IPPROTO_ICMPV6*/
};

enum class type
{
stream = SOCK_STREAM,
datagrams =SOCK_DGRAM,
sequence_packet = SOCK_SEQPACKET,
raw = SOCK_RAW,
reliable_datagram_layer=SOCK_RDM,
};

enum class protocal
{
none = 0,
ipv4 = IPPROTO_IP,
ipv6 = IPPROTO_IPV6,
tcp = IPPROTO_TCP,
udp = IPPROTO_UDP,
};

}

class address
{
	std::uint16_t prt;
	std::string_view add;
public:
	constexpr address(std::uint16_t port,std::string_view a=std::string_view()):prt(port),add(a){}
	constexpr auto port() const {return prt;}
	constexpr auto addr() const {return add;}
};

}
#ifdef __WINNT__
#include "win32_socket.h"
#else
#include "posix_socket.h"
#endif