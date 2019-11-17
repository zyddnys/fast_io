#include<iostream>
#include"../../include/fast_io.h"
#include"ryu.h"
int main()
{
	constexpr double v(0.000000000000000012393542857386751165905);
	std::cout<<v<<std::endl;
	print(fast_io::out,"fast_io:",v,"\nryu source:");
	std::array<char,1024> arr;
	auto p(arr.data()+d2s_buffered_n(v,arr.data()));
	*p='\n';
	writes(fast_io::out,arr.data(),++p);
}
