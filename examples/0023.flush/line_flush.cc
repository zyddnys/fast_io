#include"../../include/fast_io.h"

int main()
{
	fast_io::line_flush<fast_io::basic_obuf<fast_io::system_io_handle>> lnflush(fast_io::native_stdout_number);
	fast_io::basic_ibuf<fast_io::system_io_handle> ib(fast_io::native_stdin_number);
	print(lnflush,"Please input 2 numbers\n");
	std::size_t a,b;
	scan(ib,a,b);
	println(lnflush,"sum of a+b = ",a+b);
}