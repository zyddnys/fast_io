#include<iomanip>
#include<iostream>
#include"../../include/fast_io.h"

int main()
{
	auto v(1621.68796996734999993350);
	println(fast_io::out,fast_io::fixed(v,10));
	std::cout<<"hw:"<<std::fixed<<std::setprecision(10)<<v<<"\n";
}