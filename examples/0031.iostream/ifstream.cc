#include"../../include/fast_io.h"
#include<fstream>

int main()
try
{
	std::ifstream fin("ifstream.txt",std::ofstream::binary);
	fast_io::stream_view in(fin);
	std::size_t v(0);
	scan(in,v);
	println(fast_io::out,v);
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}