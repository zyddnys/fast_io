#include"../../include/fast_io.h"
#include"timer.h"

int main()
{
	std::size_t constexpr N(1000000000);
	{
		cqw::timer tmr("fill_nc ostring");
		fast_io::ostring ob("fill_nc.txt");
//		for(std::size_t i(0);i!=N;++i)
			fill_nc(ob,N,'0');
	}
	{
		cqw::timer tmr("loop_put ostring");
		fast_io::ostring ob("fill_nc.txt");
		for(std::size_t i(0);i!=N;++i)
//			for(std::size_t j(0);j!=s;++j)
				put(ob,'0');
	}
}