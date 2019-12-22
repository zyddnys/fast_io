#include"timer.h"
#include<fstream>
#include"../../include/fast_io.h"
#include<exception>
#include<cmath>
#include<memory>
#include<cstdio>
#include<vector>

int main()
try
{
	std::size_t constexpr N(10000);
	std::vector<std::size_t> vec(10000,5);
//THIS IS A WRONG BENCHMARK!!! You can basically manipulate the result by changing order
	{
	cqw::timer t("std::FILE*");
	std::unique_ptr<std::FILE,decltype(fclose)*> fp(std::fopen("cfilestar.txt","wb"),fclose);
	for(std::size_t i(0);i!=N;++i)
		fwrite(vec.data(),sizeof(decltype(vec)::value_type),vec.size(),fp.get());
	}
	{
	cqw::timer t("std::ofstream");
	std::ofstream fout("ofstream.txt",std::ofstream::binary);
	for(std::size_t i(0);i!=N;++i)
		fout.write(static_cast<char const*>(static_cast<void const*>(vec.data())),vec.size()*sizeof(decltype(vec)::value_type));

	}
	{
	cqw::timer t("std::ofstream buf");
	std::ofstream fout("ofstream_buf.txt",std::ofstream::binary);
	auto &rdbuf(*fout.rdbuf());
	for(std::size_t i(0);i!=N;++i)
		rdbuf.sputn(static_cast<char const*>(static_cast<void const*>(vec.data())),vec.size()*sizeof(decltype(vec)::value_type));
	}
	{
	cqw::timer t("obuf");
	fast_io::obuf obuf("obuf.txt");
	for(std::size_t i(0);i!=N;++i)
		send(obuf,vec.cbegin(),vec.cend());
	}
	{
	cqw::timer t("obuf text");
	fast_io::text_view<fast_io::obuf> view("obuf_text.txt");
	for(std::size_t i(0);i!=N;++i)
		send(view,vec.cbegin(),vec.cend());
	}
	{
	cqw::timer t("steam_view for ofstream");
	std::ofstream fout("stream_view_ofstream.txt",std::ofstream::binary);
	fast_io::stream_view view(fout);
	for(std::size_t i(0);i!=N;++i)
		send(view,vec.cbegin(),vec.cend());
	}
	{
	cqw::timer t("steambuf_view for ofstream");
	std::ofstream fout("streambuf_view_ofstream.txt",std::ofstream::binary);
	fast_io::streambuf_view view(fout.rdbuf());
	for(std::size_t i(0);i!=N;++i)
		send(view,vec.cbegin(),vec.cend());
	}
	{
	cqw::timer t("dynamic obuf");
	fast_io::dynamic_character_output_stream dobuf(std::in_place_type<fast_io::obuf>,"dynamic_obuf.txt");
	for(std::size_t i(0);i!=N;++i)
		send(dobuf,vec.cbegin(),vec.cend());
	}
	{
	cqw::timer t("iobuf_dynamic system_file");
	fast_io::iobuf_dynamic dobuf(std::in_place_type<fast_io::osystem_file>,"iobuf_dynamic_system_file.txt");
	for(std::size_t i(0);i!=N;++i)
		send(dobuf,vec.cbegin(),vec.cend());
	}
	{
	cqw::timer t("obuf_mutex");
	fast_io::obuf_mutex obuf("obuf_mutex.txt");
	for(std::size_t i(0);i!=N;++i)
		send(obuf,vec.cbegin(),vec.cend());
	}
	{
	cqw::timer t("ofsync");
	fast_io::ofsync obuf("ofsync.txt",fast_io::open::interface<fast_io::open::out|fast_io::open::trunc>);
	for(std::size_t i(0);i!=N;++i)
		send(obuf,vec.cbegin(),vec.cend());
	}
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}