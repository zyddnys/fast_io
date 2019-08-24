#include"../include/fast_io.h"
#include<exception>
#include<iostream>

int main()
try
{
	fast_io::ibuf ib("not_existing_file.txt");
}
catch(std::exception const& e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}