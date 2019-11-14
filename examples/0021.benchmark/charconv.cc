#include"../../include/fast_io_device.h"
#include"timer.h"
#include<charconv>

int main()
{
	std::size_t constexpr N(10000000);
	{
	cqw::timer t("obuf");
	fast_io::obuf obuf("obuf.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,fast_io::fixed<6>(i));
	}

	{
	cqw::timer t("charcv");
	fast_io::obuf obuf("obuf_std_to_chars.txt");
	std::array<char, 50> buffer;
	for(std::size_t i(0);i!=N;++i)
	{
		auto [p,ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(),i,std::chars_format::fixed,6);
		*p='\n';
		writes(obuf,buffer.data(),++p);
	}
	}
}