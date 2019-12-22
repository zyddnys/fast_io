#include"../../include/fast_io.h"

int main()
{
	std::size_t sum(0);
	for(std::size_t i(0);i!=100;++i)
	{
		sum+=i;
		if(i==50)
			goto nextloop;
	}
nextloop:;
	for(std::size_t i(0);i!=100;++i)
	{
		sum+=i;
	}

	println(fast_io::out,sum);
}