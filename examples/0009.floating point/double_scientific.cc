#include<iostream>
#include<iomanip>
#include"../../include/fast_io.h"

int main()
{
	constexpr double v(3.93542857386751165905);
	std::cout<<"std::cout: "<<std::scientific<<std::setprecision(20)<<v<<std::endl;
	println(fast_io::out,"fast_io: ",fast_io::scientific<20,false>(v));
}