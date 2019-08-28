#include"../../include/fast_io.h"

int main()
{
	fast_io::line_flush<fast_io::basic_obuf<fast_io::posix_io_handle>> lnflush(1);	//STDOUT_FILENO
	fast_io::basic_ibuf<fast_io::posix_io_handle> ib(0);			//STDIN_FILENO
	print(lnflush,"Please input 2 numbers\n");
	std::size_t a,b;
	scan(ib,a,b);
	println(lnflush,"sum of a+b = ",a+b);
}