#include"../../include/fast_io.h"
#include"../../include/fast_io_network.h"

int main()
try
{
	for(auto const& address:fast_io::dns("www.google.com"))
		println(fast_io::out, address);
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}
