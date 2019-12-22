#include"../../include/fast_io.h"

int main()
{
	fast_io::char_flush<fast_io::basic_obuf<fast_io::system_io_handle>,'\t'> tab_flush(fast_io::native_stdout_number);	//'\t' flush
	fast_io::basic_ibuf<fast_io::system_io_handle> ib(fast_io::native_stdin_number);			//STDIN_FILENO
	print(tab_flush,"Please input 2\t numbers\n");				//we set '\t' for flushing
	std::size_t a,b;
	scan(ib,a,b);
	println(tab_flush,"sum of a+b = ",a+b);
}