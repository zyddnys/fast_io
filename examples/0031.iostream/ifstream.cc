#include"../../include/fast_io.h"
#include<fstream>

int main()
try
{
	fast_io::stream_view<std::ifstream> in("ifstream.txt",std::ofstream::binary);
	std::size_t v(0);
	scan(in,v);
	println(fast_io::out,v);
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}