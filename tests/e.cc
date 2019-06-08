#include"timer.h"
#include<fstream>
#include"../include/fast_io.h"
#include<exception>
#include<cmath>
#include<iomanip>
#include<memory>
#include<cstdio>
#include"iomutex2.h"

int main()
try
{
	auto constexpr pi(4*std::atan(1));
	std::size_t constexpr N(10000000);
	{
	cqw::timer t("ofstream");
	std::ofstream fout("ofstream.txt",std::ofstream::binary);
	decltype(auto) rdbuf(*fout.rdbuf());
	fout<<std::fixed<<std::setprecision(10);
	for(std::size_t i(0);i!=N;++i)
	{
		fout<<pi;
		rdbuf.sputc('\n');
	}
	}
	{
	cqw::timer t("std::FILE*");
	std::unique_ptr<std::FILE,decltype(fclose)*> fp(std::fopen("cfilestar.txt","wb"),fclose);
	for(std::size_t i(0);i!=N;++i)
		fprintf(fp.get(),"%.10f\n",pi);
	}
	{
	cqw::timer t("obuf");
	fast_io::obuf obuf("obuf.txt");
	for(std::size_t i(0);i!=N;++i)
		print(obuf,fast_io::fixed(pi,10),fast_io::character('\n'));
	}
	{
	cqw::timer t("obuf_fprint");
	fast_io::obuf obuf("obuf_fprint.txt");
	for(std::size_t i(0);i!=N;++i)
		fprint(obuf,"%\n",fast_io::fixed(pi,10));
	}
	{
	cqw::timer t("obuf_mutex");
	fast_io::obuf_mutex obuf("obuf_mutex.txt");
	for(std::size_t i(0);i!=N;++i)
		print(obuf,fast_io::fixed(pi,10),fast_io::character('\n'));
	}
	
	{
	cqw::timer t("obuf_mutex2");
	fast_io::obuf_mutex2 obuf("obuf_mutex2.txt");
	for(std::size_t i(0);i!=N;++i)
		print(obuf,fast_io::fixed(pi,10),fast_io::character('\n'));
	}
}
catch(std::exception const& e)
{
	return 1;
}