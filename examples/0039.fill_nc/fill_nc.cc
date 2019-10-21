#include"../../include/fast_io.h"
#include"timer.h"

int main()
{
	std::size_t constexpr s(100000000);
	{
		cqw::timer tmr("fill_nc");
		fast_io::obuf ob("fill_nc.txt");
		fill_nc(ob,s,'0');
	}
	{
		cqw::timer tmr("loop_put");
		fast_io::obuf ob("loop_put.txt");
		for(std::size_t i(0);i!=s;++i)
			ob.put('0');
	}
}