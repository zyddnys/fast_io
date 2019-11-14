#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<vector>

int main()
try
{
	fast_io::ibuf ib("in.txt");
	std::vector<std::size_t> vec;
	try
	{
		for(;;)
		{
			std::size_t a;
			scan(ib,a);
			vec.emplace_back(a);
		}
	}
	catch(fast_io::eof const&){}
	println(fast_io::out,vec.size());
}
catch(std::exception const &e)
{
	println(fast_io::err,e);
	return 1;
}