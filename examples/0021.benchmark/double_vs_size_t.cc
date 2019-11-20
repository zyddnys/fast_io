#include"timer.h"
#include<vector>

int main()
{
	constexpr std::size_t N(1);
	std::vector<std::size_t> vec(7,12412421);
	vec[6]=325;
	{
		cqw::timer t1("size_t");
		std::size_t sum(0);
		for(std::size_t i(0);i!=N;++i)
		for(auto const& e : vec)
			sum+=e/vec[6];
		printf("%zu\n",sum);
	}
	std::vector<double> vec1(1000,12412421);
	vec1[6]=325;
	{
		cqw::timer t1("double");
		std::size_t sum(0);
		for(std::size_t i(0);i!=N;++i)
		for(auto const& e : vec1)
			sum+=e/vec1[6];
		printf("%g\n",sum);
	}
}