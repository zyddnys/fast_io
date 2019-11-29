#include"../../include/fast_io.h"
#include"../../include/fast_io_network.h"


int main()
try
{
	fast_io::server hd(2000,fast_io::sock::type::stream);
	for(;;)
	try
	{
		fast_io::acceptor_buf accept(hd);
		print(accept,"hello world\n");
		println(fast_io::out,"a client connect to this server");
	}
	catch(std::exception const & e)
	{
		println(fast_io::err,e);
	}
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}