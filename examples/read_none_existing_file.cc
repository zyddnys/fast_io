#include"../include/fast_io.h"

int main()
try
{
	fast_io::ibuf ib("not_existing_file.txt");
}
catch(std::exception const& e)
{
	fprint(fast_io::err,"%\n",e.what());
	return 1;
}