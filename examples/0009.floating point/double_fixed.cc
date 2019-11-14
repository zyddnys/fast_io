#include<iostream>
#include"../../include/fast_io.h"
#include"ryu.h"

int main()
{
//	auto v(1621.68796996734999993350);
	auto v(123.93542857386751165905);
	println(fast_io::out,"my implementation ",fast_io::fixed<20>(v));
	print(fast_io::out,"ryu implementation ");
	std::array<char,1024> arr;
	auto p(arr.data()+d2fixed_buffered_n(v,20,arr.data()));
	*p='\n';
	writes(fast_io::out,arr.data(),++p);
}