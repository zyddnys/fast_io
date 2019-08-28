#include"../../include/fast_io.h"

int main()
{
	fast_io::char_flush<fast_io::basic_obuf<fast_io::posix_io_handle>,'\t'> tab_flush(1);	//'\t' flush
	fast_io::basic_ibuf<fast_io::posix_io_handle> ib(0);			//STDIN_FILENO
	print(tab_flush,"Please input 2\t numbers\n");
	std::size_t a,b;
	scan(ib,a,b);
	println(tab_flush,"sum of a+b = ",a+b);
}