#include"../../include/fast_io.h"
#include"timer.h"
#include<charconv>

int main()
{
	std::size_t constexpr N(10000000);
	{
	cqw::timer t("obuf");
	fast_io::obuf obuf("obuf_cv.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,i);
	}

	{
	cqw::timer t("charcv");
	fast_io::obuf obuf("charcv.txt");
	std::array<char, 50> buffer;
	for(std::size_t i(0);i!=N;++i)
	{
		auto [p,ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(),i);
		*p='\n';
		obuf.writes(buffer.data(),++p);
	}
	}
}