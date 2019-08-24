#include"../include/fast_io.h"
#include<cstddef>

int main()
{
	std::size_t a,b;
	print(fast_io::system_out,"Please input 2 numbers\n");
	fast_io::system_in>>a>>b;
	fprint(fast_io::system_out,"sum of a+b = %\n",a+b);
	std::size_t c;

}