#include"../../include/fast_io.h"

int main()
{
	std::size_t v;
	scan(fast_io::in,v);
	println(fast_io::out,fast_io::scientific(v,3));
}