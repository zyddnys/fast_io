#include"../../include/fast_io.h"
#include<iostream>
#include<iomanip>

int main()
{
	double v;
	scan(fast_io::in,v);
	println(fast_io::out,fast_io::fixed(0.0,5));
}