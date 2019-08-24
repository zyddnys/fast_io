#include"../include/fast_io.h"
#include<cstddef>

int main()
{
	std::size_t a,b;
	fast_io::system_in>>a>>b;
	fprint(fast_io::system_out,"sum of a+b = %\n",a+b);
}