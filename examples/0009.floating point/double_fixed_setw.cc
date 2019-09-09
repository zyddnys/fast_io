#include"../../include/fast_io.h"

int main()
{
	double v;
	scan(fast_io::in,v);
	println(fast_io::out,fast_io::setw(10,fast_io::fixed(v,3)));
}