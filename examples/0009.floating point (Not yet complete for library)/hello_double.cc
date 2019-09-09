#include<iostream>
#include"../../include/fast_io.h"

int main()
{
	double v;
	scan(fast_io::in,v);
//	std::cout<<v<<std::endl;
	fprint(fast_io::out,"%\n",fast_io::scientific(v,4));
}