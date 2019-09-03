#include"../../include/fast_io.h"

int main()
try
{
	fast_io::win32_file file("abc.txt","wx");
//	println(file,"hello world");
}
catch(std::exception const & e)
{
	println(fast_io::out,e);
	return 1;
}