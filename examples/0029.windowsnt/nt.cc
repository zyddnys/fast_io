#include"../../include/fast_io.h"

int main()
try
{
	fast_io::obuf file("abc.txt");
	println(file,"hello world");
}
catch(std::exception const & e)
{
	println(fast_io::out,e);
	return 1;
}