#include"../../include/fast_io.h"
#include<iostream>

int main()
try
{
	fast_io::stream_view cout(std::cout);
	println(cout,"Hello World\n");
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}