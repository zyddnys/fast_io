#include"timer.h"
#include<fstream>
#include"../../include/fast_io.h"
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
	cqw::timer t("obuf");
	fast_io::obuf obuf("obuf.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,i);
	}
	{
	cqw::timer t("obuf text");
	fast_io::obuf obuf("obuf_text.txt");
	fast_io::text_view view(obuf);
	for(std::size_t i(0);i!=N;++i)
		println(view,i);
	}
	{
	cqw::timer t("steam_view for ofstream");
	std::ofstream fout("stream_view_ofstream.txt",std::ofstream::binary);
	fast_io::stream_view view(fout);
	for(std::size_t i(0);i!=N;++i)
		println(view,i);
	}
	{
	cqw::timer t("steambuf_view for ofstream");
	std::ofstream fout("streambuf_view_ofstream.txt",std::ofstream::binary);
	fast_io::streambuf_view view(fout.rdbuf());
	for(std::size_t i(0);i!=N;++i)
		println(view,i);
	}
	{
	cqw::timer t("obuf ucs_view");
	fast_io::obuf obuf("obuf_ucsview.txt");
	fast_io::ucs_view<decltype(obuf),char32_t> uv(obuf);
	for(std::size_t i(0);i!=N;++i)
		println(uv,i);
	}
	{
	cqw::timer t("dynamic obuf");
	fast_io::dynamic_standard_output_stream dobuf(std::in_place_type<fast_io::obuf>,"dynamic_obuf.txt");
	for(std::size_t i(0);i!=N;++i)
		println(dobuf,i);
	}
	{
	cqw::timer t("iobuf_dynamic system_file");
	fast_io::iobuf_dynamic dobuf(std::in_place_type<fast_io::osystem_file>,"iobuf_dynamic_system_file.txt");
	for(std::size_t i(0);i!=N;++i)
		println(dobuf,i);
	}
	{
	cqw::timer t("obuf_mutex");
	fast_io::obuf_mutex obuf("obuf_mutex.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,i);
	}
	{
	cqw::timer t("ofsync");
	fast_io::ofsync obuf("ofsync.txt",fast_io::open::interface<fast_io::open::out|fast_io::open::trunc>);
	for(std::size_t i(0);i!=N;++i)
		println(obuf,i);
	}
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}