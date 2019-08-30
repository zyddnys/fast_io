#include"../../include/fast_io.h"

int main()
{
	fast_io::ioposix_pipe pipe;
	println(pipe,"hello world ",4);
	std::size_t v;
	scan(pipe,v);
}