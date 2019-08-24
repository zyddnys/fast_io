#include"timer.h"
#include<fstream>
#include<iostream>
#include"../include/fast_io.h"
#include<exception>
#include<cmath>
#include<memory>
#include<cstdio>

int main()
try
{
	std::size_t constexpr N(10000000);
	{
	cqw::timer t("std::FILE*");
	std::unique_ptr<std::FILE,decltype(fclose)*> fp(std::fopen("cfilestar.txt","wb"),fclose);
	for(std::size_t i(0);i!=N;++i)
		fprintf(fp.get(),"%zu\n",i);
	}
	{
	cqw::timer t("obuf");
	fast_io::obuf obuf("obuf.txt",fast_io::open::interface<fast_io::open::binary>);
	for(std::size_t i(0);i!=N;++i)
		(obuf<<i).put('\n');
	}
	{
	cqw::timer t("hacked c_style file");
	fast_io::c_style_file csf("hack.txt",fast_io::open::interface<fast_io::open::out|fast_io::open::binary>);
	for(std::size_t i(0);i!=N;++i)
		(csf<<i).put('\n');
	}
/*	{
	cqw::timer t("obuf_mutex");
	fast_io::obuf_mutex obuf("obuf_mutex.txt");
	for(std::size_t i(0);i!=N;++i)
		print(obuf,i,fast_io::character('\n'));
	}*/
}
catch(std::exception const& e)
{
	return 1;
}