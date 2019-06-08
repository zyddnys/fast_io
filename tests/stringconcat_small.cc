#include"timer.h"
#include<fstream>
#include"../include/fast_io.h"
#include<exception>
#include<cmath>
#include<iomanip>
#include<memory>
#include<cstdio>
#include<sstream>
#include<string_view>

int main()
try
{

	using namespace std::string_literals;
	using namespace std::string_view_literals;
	std::size_t constexpr N(50000000);
	std::string const baseLocation;
	{
		cqw::timer t("std::string");
		std::string fname;
		for(std::size_t i(0);i!=N;++i)
		{
			fname.append(baseLocation + std::to_string(i) + ".t");
			fname.clear();
		}
	}
	{
		cqw::timer t("std::string append");
		std::string fname;
		for(std::size_t i(0);i!=N;++i)
		{
			fname.append(baseLocation);
			fname.append(std::to_string(i));
			fname.append(".t"sv);
			fname.clear();
		}
	}
	{
		cqw::timer t("sstream");
		std::ostringstream sst;
		for(std::size_t i(0);i!=N;++i)
		{
			sst<<baseLocation<<i<<".t";
			sst.clear();
		}
	}
	{
		cqw::timer t("snprintf");
		for(std::size_t i(0);i!=N;++i)
		{
			std::array<char,255> fname{0};
			snprintf(fname.data(), 255, "%s%d.t", baseLocation.c_str(), i);
		}
	}
	{
		cqw::timer t("snprintf2");
		std::array<char,255> fname{0};
		for(std::size_t i(0);i!=N;++i)
			snprintf(fname.data(), 255, "%s%d.t", baseLocation.c_str(), i);
	}
	{
		cqw::timer t("obuf_string");
		fast_io::obuf_string obuf;
		for(std::size_t i(0);i!=N;++i)
		{
			obuf<<baseLocation<<i<<".t";
			obuf.clear();
		}
	}
	{
		cqw::timer t("obuf_string fprint");
		fast_io::obuf_string obuf;
		for(std::size_t i(0);i!=N;++i)
		{
			fprint(obuf,"%%.t",baseLocation,i);
			obuf.clear();
		}
	}
	{
		cqw::timer t("fast_io::concat");
		for(std::size_t i(0);i!=N;++i)
			fast_io::concat<std::string>(baseLocation,i,".t");
	}
}
catch(std::exception const& e)
{
	return 1;
}