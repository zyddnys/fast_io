#include<cmath>
#include"timer.h"
#include<cstdio>
#include<random>

inline constexpr double mpow10(double const f,std::size_t u)
{
	if(u==0)
		return 1;
	else if(u==1)
		return f;
	else
	{
		double d(mpow10(f,u>>1));
		d*=d;
		if(u&1)
			return d*f;
		else
			return d;
	}
}

int main()
{
	std::size_t N(10000000);
	std::vector<std::size_t> vec;
	vec.reserve(N);
	std::random_device device;
	std::mt19937_64 eng(device());
	std::uniform_int_distribution<std::size_t> dis(0,99);
	for(std::size_t i(0);i!=N;++i)
		vec.emplace_back(dis(eng));
{
	double sum(0);
	cqw::timer t("cmath");
	for(std::size_t i(0);i!=N;++i)
		if(i)
			sum-=std::pow(10.0,vec[i]);
		else
			sum+=std::pow(10.0,vec[i]);
	fprintf(stderr,"%f\n",sum);
}

{
	cqw::timer t("mpow10");
	double sum(0);
	for(std::size_t i(0);i!=N;++i)
		if(i)
			sum-=mpow10(10.0,vec[i]);
		else
			sum+=mpow10(10.0,vec[i]);
	fprintf(stderr,"%f\n",sum);

}

}