#include"../../include/fast_io.h"

int main()
{
	fast_io::pipe pipe;
	println(pipe,"hello world ",4);
	std::size_t v;
	scan(pipe,v);
	println(fast_io::out,v);
}