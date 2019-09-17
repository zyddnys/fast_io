#include"../../include/fast_io.h"

int main()
try
{
	fast_io::client_buf hd(fast_io::sock::family::ipv4,fast_io::address(2000,"172.29.147.123"),fast_io::sock::type::stream);
	std::string str;
	getwhole(hd,str);
	println(fast_io::out,str);
}
catch(std::system_error const & e)
{
	println(fast_io::err,e);
	return 1;
}