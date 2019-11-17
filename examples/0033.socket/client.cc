#include"../../include/fast_io.h"
#include"../../include/fast_io_network.h"

int main()
try
{
	fast_io::client hd(fast_io::to<fast_io::ipv4>("127.0.0.1"),2000,fast_io::sock::type::stream);
	auto native_stdout{fast_io::native_stdout()};
	transmit(native_stdout,hd);
}
catch(std::system_error const & e)
{
	println(fast_io::err,e);
	return 1;
}