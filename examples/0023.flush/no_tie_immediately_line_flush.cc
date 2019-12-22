#include"../../include/fast_io.h"

int main()
{
	fast_io::basic_obuf<fast_io::system_io_handle> un_tied_output(fast_io::native_stdout_number);
	fast_io::basic_ibuf<fast_io::system_io_handle> ib(fast_io::native_stdin_number);
	print(un_tied_output,"Please input 2 numbers\n");
	std::size_t a,b;
	scan(ib,a,b);
	println(un_tied_output,"sum of a+b = ",a+b);
}