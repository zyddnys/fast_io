#include"../../include/fast_io.h"
#include<fstream>

int main()
try
{
	std::ofstream fout("ofstream.txt");
	fast_io::stream_view fout_view(fout);
	println(fout_view,"Hello World\n");
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}