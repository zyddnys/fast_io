#include"timer.h"
#include<fstream>
#include"../include/fast_io.h"
#include<exception>
#include<cmath>
#include<iomanip>
#include<memory>
#include<cstdio>
#include<sstream>

int main()
try
{
	std::size_t constexpr N(10000000);
	{
		cqw::timer t("sstream");
		std::ostringstream sst;
		auto& rdbuf(*sst.rdbuf());
		for(std::size_t i(0);i!=N;++i)
		{
			sst<<i;
			rdbuf.sputc('\n');
		}
	}
	{
		cqw::timer t("snprintf");
		std::array<char,200> a;
		for(std::size_t i(0);i!=N;++i)
			snprintf(a.data(),a.size(),"%zu\n",i);
	}
	{
		cqw::timer t("obuf_string");
		fast_io::obuf_string obuf;
		for(std::size_t i(0);i!=N;++i)
			(obuf<<i).put('\n');
	}
	{
		cqw::timer t("obuf_string print");
		fast_io::obuf_string obuf;
		for(std::size_t i(0);i!=N;++i)
			print(obuf,i,fast_io::character('\n'));
	}
	{
		cqw::timer t("obuf_string fprint");
		fast_io::obuf_string obuf;
		for(std::size_t i(0);i!=N;++i)
			fprint(obuf,"%\n",i);
	}
}
catch(std::exception const& e)
{
	return 1;
}