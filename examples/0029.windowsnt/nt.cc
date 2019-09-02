#include"../../include/fast_io.h"

int main()
try
{
	fast_io::nt_file file("abc.txt");
}
catch(std::exception const & e)
{
	println(fast_io::out,e);
	return 1;
}