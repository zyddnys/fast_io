//Please run output_10M_size_t before this

#include"timer.h"
#include<fstream>
#include"../../include/fast_io.h"
#include<exception>
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
		auto const ret(fscanf(fp.get(),"%zu",v.data()+i));
	}
/*	{
	cqw::timer t("std::FILE* with 1048576 buffer size + _IOFBF (Full buffering) tag");
	std::unique_ptr<std::FILE,decltype(fclose)*> fp(std::fopen("cfilestar.txt","rb"),fclose);
	auto buffer(std::make_unique<char[]>(1048576));
	setvbuf(fp.get(),buffer.get(),_IOFBF,1048576);
	for(std::size_t i(0);i!=N;++i)
		auto const ret(fscanf(fp.get(),"%zu",v.data()+i));
	}*/
	{
	cqw::timer t("std::ifstream");
	std::ifstream fin("cfilestar.txt",std::ifstream::binary);
	for(std::size_t i(0);i!=N;++i)
		fin>>v[i];
	}
	{
	cqw::timer t("stream_view for std::ifstream");
	fast_io::stream_view<std::ifstream> view("cfilestar.txt",std::ifstream::binary);
	for(std::size_t i(0);i!=N;++i)
		scan(view,v[i]);
	}
	{
	cqw::timer t("streambuf_view for std::ifstream");
	std::ifstream fin("cfilestar.txt",std::ifstream::binary);
	fast_io::streambuf_view view(fin.rdbuf());
	for(std::size_t i(0);i!=N;++i)
		scan(view,v[i]);
	}
	{
	cqw::timer t("ibuf");
	fast_io::ibuf ibuf("cfilestar.txt");
	for(std::size_t i(0);i!=N;++i)
		scan(ibuf,v[i]);
	}
	{
	cqw::timer t("ibuf_mutex");
	fast_io::ibuf_mutex ibuf("cfilestar.txt");
	for(std::size_t i(0);i!=N;++i)
		scan(ibuf,v[i]);
	}
	{
	cqw::timer t("ibuf text");
	fast_io::text_view<fast_io::ibuf> view("obuf_text.txt");
	for(std::size_t i(0);i!=N;++i)
		scan(view,v[i]);
	}
/*	{
	cqw::timer t("dynamic standard input stream ibuf");
	fast_io::dynamic_character_input_stream ibuf(std::in_place_type<fast_io::ibuf>,"cfilestar.txt");
	for(std::size_t i(0);i!=N;++i)
		scan(ibuf,v[i]);
	}
	{
	cqw::timer t("ibuf_dynamic isystem_file");
	fast_io::ibuf_dynamic ibuf(std::in_place_type<fast_io::isystem_file>,"cfilestar.txt");
	for(std::size_t i(0);i!=N;++i)
		scan(ibuf,v[i]);
	}*/
	{
	cqw::timer t("speck128/128");
	fast_io::crypto::basic_ictr<fast_io::ibuf, fast_io::crypto::speck::speck_enc_128_128> enc_stream(
		std::array<uint8_t, 16>{'8','3','3','4',';','2','3','4','a','2','c','4',']','0','3','4'},
		std::array<uint8_t, 8>{'1','2','3','4','1','2','3','4'},"speck.txt");
	for(auto & e : v)
		scan(enc_stream,e);
	}
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}