#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_hash.h"

int main(int argc,char ** argv)
try
{
	if(argc!=2)
	{
		print(fast_io::err,"Usage: ",*argv," <path>");
		return 1;
	}
	fast_io::isystem_file ib(argv[1]);
	fast_io::sha1 sha1;
	transmit(sha1,ib);
	flush(sha1);
	println(fast_io::out,sha1);
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}