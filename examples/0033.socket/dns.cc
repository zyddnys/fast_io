#include"../../include/fast_io.h"
#include"../../include/fast_io_network.h"

int main(int argc,char** argv)
try
{
	if(argc!=2)
	{
		println(fast_io::err,"Usage: ",*argv," <domain>");
		return 1;
	}
	for(auto const& address:fast_io::dns(argv[1]))
		println(fast_io::out, address);
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}
