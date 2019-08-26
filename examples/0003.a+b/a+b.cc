#include"../../include/fast_io.h"
#include<cstddef>

int main()
{
	fast_io::out<<"Please input 2 numbers\n";
	std::size_t a,b;
	fast_io::in>>a>>b;
	println(fast_io::out,"sum of a+b = ",a+b);
}