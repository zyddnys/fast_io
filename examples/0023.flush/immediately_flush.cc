#include"../../include/fast_io.h"

int main()
{
	fast_io::immediately_flush<fast_io::basic_obuf<fast_io::posix_io_handle>> un_tied_output(1);	//STDOUT_FILENO
	fast_io::basic_ibuf<fast_io::posix_io_handle> ib(0);			//STDIN_FILENO
	print(un_tied_output,"Please input 2 numbers\n");
	std::size_t a,b;
	scan(ib,a,b);
	println(un_tied_output,"sum of a+b = ",a+b);
}