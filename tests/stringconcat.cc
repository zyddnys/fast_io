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
	std::size_t constexpr N(10000000);
	std::string const baseLocation("127.0.0.1");
	{
		cqw::timer t("std::string");
		std::string fname;
		for(std::size_t i(0);i!=N;++i)
		{
			fname.append(baseLocation + "_test_no." + std::to_string(i) + ".txt");
			fname.clear();
		}
	}
	{
		cqw::timer t("std::string");
		std::string fname;
		for(std::size_t i(0);i!=N;++i)
		{
			fname.append(baseLocation);
			fname.append("_test_no."sv);
			fname.append(std::to_string(i));
			fname.append(".txt"sv);
			fname.clear();
		}
	}
	{
		cqw::timer t("sstream");
		std::ostringstream sst;
		for(std::size_t i(0);i!=N;++i)
		{
			sst<<baseLocation<<"_test_no"<<i<<".txt";
			sst.clear();
		}
	}
	{
		cqw::timer t("snprintf");
		for(std::size_t i(0);i!=N;++i)
		{
			std::array<char,255> fname{0};
			snprintf(fname.data(), 255, "%s_test_no.%d.txt", baseLocation.c_str(), i);
		}
	}
	{
		cqw::timer t("snprintf2");
		std::array<char,255> fname{0};
		for(std::size_t i(0);i!=N;++i)
			snprintf(fname.data(), 255, "%s_test_no.%d.txt", baseLocation.c_str(), i);
	}
	{
		cqw::timer t("obuf_string");
		fast_io::obuf_string obuf;
		for(std::size_t i(0);i!=N;++i)
		{
			obuf<<baseLocation<<"_test_no."<<i<<".txt";
			obuf.str().clear();
		}
	}
	{
		cqw::timer t("obuf_string fprint");
		fast_io::obuf_string obuf;
		for(std::size_t i(0);i!=N;++i)
		{
			fprint(obuf,"%_test_no.%.txt",baseLocation,i);
			obuf.str().clear();
		}
	}

}
catch(std::exception const& e)
{
	return 1;
}