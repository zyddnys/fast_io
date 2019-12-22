#include"../../include/fast_io.h"

int main()
{
	fast_io::immediately_flush<fast_io::basic_obuf<fast_io::system_io_handle>> immeflush(fast_io::native_stdout_number);	//STDOUT_FILENO
	fast_io::basic_ibuf<fast_io::system_io_handle> ib(fast_io::native_stdin_number);			//STDIN_FILENO
	print(immeflush,"Please input 2 numbers\n");
	std::size_t a,b;
	scan(ib,a,b);
	println(immeflush,"sum of a+b = ",a+b);
}