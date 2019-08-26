#include"../../include/fast_io.h"
#include"../../include/impl/tie.h"
#include<exception>

int main()
try
{
	fast_io::obuf ob("tied.txt");
	fast_io::tie<fast_io::ibuf,fast_io::obuf> ib(ob,"not_existing_file.txt");
	ob<<"35\n";
	std::size_t d;
	ib>>d;
}
catch(std::exception const& e)
{
	fprint(fast_io::err,"%\n",e);
	return 1;
}