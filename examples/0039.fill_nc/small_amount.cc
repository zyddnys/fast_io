#include"../../include/fast_io.h"
#include"timer.h"

int main()
{
	std::size_t constexpr s(5),N(10000000);
	{
		cqw::timer tmr("fill_nc w");
		fast_io::obuf ob("fill_nc.txt");
		for(std::size_t i(0);i!=N;++i)
			fill_nc(ob,s,'0');
	}
	{
		cqw::timer tmr("loop_put w");
		fast_io::obuf ob("loop_put.txt");
		for(std::size_t i(0);i!=N;++i)
			for(std::size_t j(0);j!=s;++j)
				ob.put('0');
	}
}