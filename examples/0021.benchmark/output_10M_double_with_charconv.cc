#include"timer.h"
#include<fstream>
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_crypto.h"
#include<exception>
#include<cmath>
#include<memory>
#include<cstdio>
#include<random>
#include<iomanip>
//#include<charconv>
#include"ryu.h"

int main()
try
{
	std::size_t constexpr N(10000000);
	std::size_t constexpr M(20);
	std::vector<double> vec;
	vec.reserve(N);
	std::random_device device;
	std::mt19937_64 eng(device());
	std::uniform_real_distribution dis(-1000.0,1000.0);
	for(std::size_t i(0);i!=N;++i)
		vec.emplace_back(dis(eng));
	{
	cqw::timer t("ryu_source");
	fast_io::obuf obuf("ryu_sourcedb.txt");
	std::array<char,1000> arr;
	for(std::size_t i(0);i!=N;++i)
	{
		auto p(arr.data()+d2fixed_buffered_n(vec[i],static_cast<std::uint32_t>(M),arr.data()));
		*p='\n';
		writes(obuf,arr.data(),++p);
	}
	}
	{
	cqw::timer t("obuf");
	fast_io::obuf obuf("obufdb.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,fast_io::fixed<M>(vec[i]));
	}
	{
	cqw::timer t("obuf_mutex");
	fast_io::obuf_mutex obuf("obuf_mutexdb.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,fast_io::fixed<M>(vec[i]));
	}
	{
	cqw::timer t("speck128/128");
	fast_io::crypto::basic_octr<fast_io::obuf, fast_io::crypto::speck::speck_enc_128_128> enc_stream(
		std::array<uint8_t, 16>{'8','3','3','4',';','2','3','4','a','2','c','4',']','0','3','4'},
		std::array<uint8_t, 8>{'1','2','3','4','1','2','3','4'},"speckdb.txt");
	for(std::size_t i(0);i!=N;++i)
		println(enc_stream,fast_io::fixed<M>(vec[i]));
	}
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}
