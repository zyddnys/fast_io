#include"timer.h"
#include<fstream>
#include"../include/fast_io.h"
#include<exception>
#include<cmath>
#include<memory>
#include<cstdio>
#include<vector>

int main()
try
{
	std::size_t constexpr N(10000000);
	std::vector<std::size_t> v(N);

	{
	cqw::timer t("std::FILE*");
	std::unique_ptr<std::FILE,decltype(fclose)*> fp(std::fopen("cfilestar.txt","rb"),fclose);
	for(std::size_t i(0);i!=N;++i)
		fscanf(fp.get(),"%zu\n",v.data()+i);
	}
	{
	cqw::timer t("std::ifstream");
	std::ifstream fin("cfilestar.txt",std::ofstream::binary);
	for(std::size_t i(0);i!=N;++i)
		fin>>v[i];
	}
	{
	cqw::timer t("ibuf");
	fast_io::ibuf ibuf("cfilestar.txt",fast_io::open::interface<fast_io::open::binary>);
	for(std::size_t i(0);i!=N;++i)
		ibuf>>v[i];
	}
	{
	cqw::timer t("dynamic standard input stream ibuf");
	fast_io::dynamic_standard_input_stream ibuf(std::in_place_type<fast_io::ibuf>,"cfilestar.txt",fast_io::open::interface<fast_io::open::binary>);
	for(std::size_t i(0);i!=N;++i)
		ibuf>>v[i];
	}

}
catch(std::exception const& e)
{
	fast_io::err<<e<<"\n";
	return 1;
}