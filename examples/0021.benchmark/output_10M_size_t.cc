#include"timer.h"
#include<fstream>
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_crypto.h"
#include<exception>
#include<cmath>
#include<memory>
#include<cstdio>
#include<charconv>

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
	cqw::timer t("std::ofstream");
	std::ofstream fout("ofstream.txt",std::ofstream::binary);
	for(std::size_t i(0);i!=N;++i)
		fout<<i<<'\n';
	}
	{
	cqw::timer t("std::ofstream with tricks");
	std::ofstream fout("ofstream_tricks.txt",std::ofstream::binary);
	auto &rdbuf(*fout.rdbuf());
	for(std::size_t i(0);i!=N;++i)
	{
		fout<<i;
		rdbuf.sputc('\n');
	}
	}
	{
	cqw::timer t("std::to_chars + ofstream rdbuf tricks");
	std::ofstream fout("ofstream_to_chars_tricks.txt",std::ofstream::binary);
	auto &rdbuf(*fout.rdbuf());
	std::array<char, 50> buffer;
	for(std::size_t i(0);i!=N;++i)
	{
		auto [p,ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(),i);
		*p='\n';
		rdbuf.sputn(buffer.data(),++p-buffer.data());
	}
	}
	{
	cqw::timer t("std::to_chars + obuf");
	fast_io::obuf obuf("std_to_chars_obuf.txt");
	std::array<char, 50> buffer;
	for(std::size_t i(0);i!=N;++i)
	{
		auto [p,ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(),i);
		*p='\n';
		writes(obuf,buffer.data(),++p);
//		put(obuf,'\n');
	}
	}
	{
	cqw::timer t("obuf");
	fast_io::obuf obuf("obuf.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,i);
	}
	{
	cqw::timer t("obuf text");
	fast_io::text_view<fast_io::obuf> view("obuf_text.txt");
	for(std::size_t i(0);i!=N;++i)
		println(view,i);
	}
	{
	cqw::timer t("obuf ucs_view");
	fast_io::ucs<fast_io::obuf,char32_t> uv("obuf_ucsview.txt");
	for(std::size_t i(0);i!=N;++i)
		println(uv,i);
	}
	{
	cqw::timer t("dynamic obuf");
	fast_io::dynamic_output_stream dobuf(std::in_place_type<fast_io::obuf>,"dynamic_obuf.txt");
	for(std::size_t i(0);i!=N;++i)
		println(dobuf,i);
	}
/*	{
	cqw::timer t("iobuf_dynamic system_file");
	fast_io::iobuf_dynamic dobuf(std::in_place_type<fast_io::osystem_file>,"iobuf_dynamic_system_file.txt");
	for(std::size_t i(0);i!=N;++i)
		println(dobuf,i);
	}*/
	{
	cqw::timer t("obuf_mutex");
	fast_io::obuf_mutex obuf("obuf_mutex.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,i);
	}
	{
	cqw::timer t("fsync");
	fast_io::fsync obuf("fsync.txt",fast_io::open::interface<fast_io::open::out|fast_io::open::trunc>);
	for(std::size_t i(0);i!=N;++i)
		println(obuf,i);
	}
	{
	cqw::timer t("speck128/128");
	fast_io::crypto::basic_octr<fast_io::obuf, fast_io::crypto::speck::speck_enc_128_128> enc_stream(
		std::array<uint8_t, 16>{'8','3','3','4',';','2','3','4','a','2','c','4',']','0','3','4'},
		std::array<uint8_t, 8>{'1','2','3','4','1','2','3','4'},"speck.txt");
	for(std::size_t i(0);i!=N;++i)
		println(enc_stream,i);
	}
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}