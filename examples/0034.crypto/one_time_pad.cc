#include<iostream>
#include"../../include/fast_io.h"

int main()
try
{
	std::string key;
	{
	fast_io::obuf ob("a.txt");
	fast_io::crypto::oone_time_pad<decltype(ob)> ootp(ob);
	print(ootp,"hello world\n");
	key=std::move(ootp.key());
	}
	{
	fast_io::ibuf ib("a.txt");
	fast_io::crypto::ione_time_pad<decltype(ib)> iotp(ib,key);
	std::string str;
	getwhole(iotp,str);
	print(fast_io::out,str);
	}
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}