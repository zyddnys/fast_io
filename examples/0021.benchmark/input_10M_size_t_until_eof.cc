//Please run output_10M_size_t before this

#include"timer.h"
#include<fstream>
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<exception>

int main()
try
{
	{
	cqw::timer t("std::ifstream");
	std::ifstream fin("cfilestar.txt",std::ifstream::binary);
	std::size_t count(0);
	for(std::size_t v;fin>>v;++count);
	println(fast_io::out,"ifstream: ",count);
	}
/*	{
	cqw::timer t("streambuf_view for std::ifstream");
	std::ifstream fin("cfilestar.txt",std::ifstream::binary);
	fast_io::streambuf_view view(fin.rdbuf());
	std::size_t count(0);
	try
	{
	for(std::size_t temp;;++count)
		scan(view,temp);
	}
	catch(fast_io::eof const &){}
	println(fast_io::out,"streambuf_view for ifstream: ",count);
	}*/
	{
	cqw::timer t("ibuf");
	fast_io::ibuf ibuf("cfilestar.txt");
	std::size_t count(0);
	try
	{
	for(std::size_t temp;;++count)
		scan(ibuf,temp);
	}
	catch(fast_io::eof const &){}
	println(fast_io::out,"ibuf: ",count);
	}
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}