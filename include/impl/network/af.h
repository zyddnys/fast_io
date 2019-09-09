#pragma once

#ifdef __WINNT__
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

namespace fast_io
{
enum class socket_family
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

enum class socket_type
{
stream = SOCK_STREAM,
datagrams =SOCK_DGRAM,
sequence_packet = SOCK_SEQPACKET,
raw = SOCK_RAW,
reliable_datagram_layer=SOCK_RDM,
};

enum class socket_protocal
{

};

/*
class socket_address
{
sockaddr_in
};*/

}
#ifdef __WINNT__
#include "win32_socket.h"
#else
#include "posix_socket.h"
#endif