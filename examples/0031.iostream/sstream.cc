#include"../../include/fast_io.h"
#include<sstream>

int main()
try
{
	fast_io::stream_view<std::stringstream> sst_view("2353256");
	std::size_t v(0);
	scan(sst_view,v);
	println(fast_io::out,v);
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}