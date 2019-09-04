#include"../../include/fast_io.h"
#include<iostream>

int main()
try
{
	fast_io::streambuf_view cout_view(std::cout.rdbuf());
	println(cout_view,"Hello World\n");
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}