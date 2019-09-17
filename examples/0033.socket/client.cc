#include"../../include/fast_io.h"

int main()
try
{
	fast_io::client_buf hd(fast_io::sock::family::ipv6,fast_io::address(2000,"2604:6000:1205:43ca:a0ee:68ea:7672:70db"),fast_io::sock::type::stream);
	std::string str;
	getwhole(hd,str);
	println(fast_io::out,str);
}
catch(std::system_error const & e)
{
	println(fast_io::err,e);
	return 1;
}