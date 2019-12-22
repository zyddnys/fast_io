#include"../../include/fast_io.h"

int main()
{
	std::size_t v(1000023523532);
	println(fast_io::out,fast_io::unsigned_view(v),"\t",fast_io::floating_view(v));
	long double v1(1000023523532);
	println(fast_io::out,fast_io::unsigned_view(v1),"\t",fast_io::floating_view(v1));
	long double p('a');
	scan(fast_io::in,fast_io::char_view(p));
	println(fast_io::out,fast_io::char_view(48.0),"\t",fast_io::char_view(p));
}