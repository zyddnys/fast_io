#include"../../include/fast_io.h"
#include<cstdlib>

int main()
{
	for(std::size_t i(0);i!=100;++i)
		std::system(fast_io::concat<>("./multithread_sync ",i," &").c_str());
}