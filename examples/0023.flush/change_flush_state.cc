#include"../../include/fast_io.h"

int main()
{
	fast_io::char_flush<fast_io::basic_obuf<fast_io::system_io_handle>,'\t'> tab_flush(fast_io::native_stdout);	//'\t' flush
	fast_io::line_flush<fast_io::basic_obuf<fast_io::system_io_handle>> line_flush(std::move(tab_flush.native_handle()));
	// Move object from tab_flush to line_flush
	fast_io::basic_ibuf<fast_io::system_io_handle> ib(fast_io::native_stdin);			//STDIN_FILENO
	print(line_flush,"Please input 2\t numbers\n");
	std::size_t a,b;
	scan(ib,a,b);
	println(line_flush,"sum of a+b = ",a+b);
}