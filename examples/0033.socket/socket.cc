#include"../../include/fast_io.h"

int main()
try
{
	fast_io::client hd(fast_io::sock::family::ipv4,fast_io::address("127.0.0.1",2000),fast_io::sock::type::stream);
}
catch(std::system_error const & e)
{
	println(fast_io::err,e);
	return 1;
}