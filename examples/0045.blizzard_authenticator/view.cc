#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"
#include"../../include/fast_io_hash.h"
#include"key_info.h"
#include<vector>

int main(int argc,char** argv)
try
{
	if(argc<2)
	{
		println(fast_io::err,"Usage : ",*argv," <filepath>");
		return 1;
	}
	fast_io::ibuf ib(argv[1]);
	std::vector<battlenet::key_info> keys;
	read(ib,keys);
	println(fast_io::out,"Local Time:\t",std::chrono::system_clock::now());
	println(fast_io::out,"Count:\t",keys.size());
	println(fast_io::out,fast_io::dec_split(keys,'\n'));
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}