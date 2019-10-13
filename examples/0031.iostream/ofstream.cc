#include"../../include/fast_io.h"
#include<fstream>

int main()
try
{
	fast_io::stream_view<std::ofstream> fout_view("ofstream.txt");
	println(fout_view,"Hello World\n");
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}