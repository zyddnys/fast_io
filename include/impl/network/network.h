#pragma once

#ifdef __WINNT__
#include <winsock2.h>
#else
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>
#endif

namespace fast_io
{
namespace sock
{
enum class family
{
unspec = AF_UNSPEC,
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
	constexpr address(std::uint16_t port,std::string_view a=""):prt(port),add(a){}
	constexpr auto port() const {return prt;}
	constexpr auto addr() const {return add;}
};

}
#ifdef __WINNT__
#include "win32_socket.h"
#else
#include "posix_socket.h"
#endif

namespace fast_io
{
using acceptor_buf = fast_io::self_tie<fast_io::basic_iobuf<fast_io::acceptor>>;
using client_buf = fast_io::self_tie<fast_io::basic_iobuf<fast_io::client>>;
using acceptor_buf_mutex = fast_io::basic_iomutex<acceptor_buf>;
using client_buf_mutex = fast_io::basic_iomutex<client_buf>;

}