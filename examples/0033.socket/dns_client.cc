#include"../../include/fast_io.h"
#include"../../include/fast_io_network.h"

int main()
try
{
	for(auto const& address:fast_io::dns("localhost"))
	{
		fast_io::client hd(address,2000,fast_io::sock::type::stream);
		auto native_stdout{fast_io::native_stdout()};
		transmit(native_stdout,hd);	//zero copy IO
	}
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}
