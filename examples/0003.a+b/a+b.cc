#include"../../include/fast_io.h"
#include<cstddef>

int main()
{
	print(fast_io::out,"Please input 2 numbers\n");
	std::size_t a,b;
	scan(fast_io::in,a,b);
	println(fast_io::out,"sum of a+b = ",a+b);
}