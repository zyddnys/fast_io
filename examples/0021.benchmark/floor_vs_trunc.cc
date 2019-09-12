#include<cmath>
#include"timer.h"
#include<cstdio>
#include<random>

int main()
{
	std::size_t N(100000000);
	std::vector<std::size_t> vec;
	vec.reserve(N);
	std::random_device device;
	std::mt19937_64 eng(device());
	std::uniform_int_distribution<std::size_t> dis(0,99);
	for(std::size_t i(0);i!=N;++i)
		vec.emplace_back(dis(eng));
{
	double sum(0);
	cqw::timer t("floor");
	for(std::size_t i(0);i!=N;++i)
		if(i)
			sum-=std::floor(vec[i]);
		else
			sum+=std::floor(vec[i]);
	fprintf(stderr,"%f\n",sum);
}

{
	cqw::timer t("trunc");
	double sum(0);
	for(std::size_t i(0);i!=N;++i)
		if(i)
			sum-=std::trunc(vec[i]);
		else
			sum+=std::trunc(vec[i]);
	fprintf(stderr,"%f\n",sum);

}

}