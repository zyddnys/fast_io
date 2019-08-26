#include"../../include/fast_io.h"
#include<exception>

int main()
try
{
	fast_io::ibuf ib("not_existing_file.txt");
}
catch(std::exception const &e)
{
	println(fast_io::err,e);
	return 1;
}