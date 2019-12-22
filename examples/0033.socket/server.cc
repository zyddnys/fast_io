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
		println(fast_io::out,"a client connect to this server");
		print_flush(accept,"hello world\n");
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