#include<exception>
#include"../include/fast_io.h"
#include"../include/impl/tie.h"

#include<iostream>

int main()
try
{
	fast_io::obuf ob("tied.txt");
	fast_io::tie<fast_io::ibuf,fast_io::obuf> ib(ob,"not_existing_file.txt");
	std::cout<<fast_io::output_stream<decltype(fast_io::tie<fast_io::ibuf,fast_io::obuf>)>()<<'\n';
}
catch(std::exception const& e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}