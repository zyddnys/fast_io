#include"../../include/fast_io.h"

int main()
try
{
	fast_io::win32_file file("abc.txt","wx");
	print(file,"hello world\n");
}
catch(std::exception const & e)
{
	println(fast_io::out,e);
	return 1;
}