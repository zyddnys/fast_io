#include<iostream>
#include"../../include/fast_io.h"
	
int main()
{
	double v;
	scan(fast_io::in,v);
	std::cout<<"iostream: "<<v<<"\n";
	println(fast_io::out,v);
}