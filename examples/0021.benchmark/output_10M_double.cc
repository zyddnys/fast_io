#include"timer.h"
#include<fstream>
#include"../../include/fast_io.h"
#include<exception>
#include<cmath>
#include<memory>
#include<cstdio>
#include<random>
#include<iomanip>

int main()
try
{
	std::size_t constexpr N(10000000);
	std::vector<double> vec;
	vec.reserve(N);
	std::random_device device;
	std::mt19937_64 eng(device());
	std::uniform_real_distribution dis(-1000.0,1000.0);
	for(std::size_t i(0);i!=N;++i)
		vec.emplace_back(dis(eng));
	{
	cqw::timer t("std::FILE*");
	std::unique_ptr<std::FILE,decltype(fclose)*> fp(std::fopen("cfilestardb.txt","wb"),fclose);
	for(std::size_t i(0);i!=N;++i)
		fprintf(fp.get(),"%.6f\n",vec[i]);
	}
	{
	cqw::timer t("std::ofstream");
	std::ofstream fout("ofstreamdb.txt",std::ofstream::binary);
	fout<<std::fixed<<std::setprecision(6);
	for(std::size_t i(0);i!=N;++i)
		fout<<vec[i]<<'\n';
	}
	{
	cqw::timer t("std::ofstream with tricks");
	std::ofstream fout("ofstream_tricksdb.txt",std::ofstream::binary);
	fout<<std::fixed<<std::setprecision(6);
	auto &rdbuf(*fout.rdbuf());
	for(std::size_t i(0);i!=N;++i)
	{
		fout<<vec[i];
		rdbuf.sputc('\n');
	}
	}
	{
	cqw::timer t("obuf");
	fast_io::obuf obuf("obufdb.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,fast_io::fixed(vec[i],6));
	}
	{
	cqw::timer t("obuf_mutex");
	fast_io::obuf_mutex obuf("obuf_mutexdb.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,fast_io::fixed(vec[i],6));
	}
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}