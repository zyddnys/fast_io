#include"../../include/fast_io.h"

int main()
{
	std::size_t ch(384989283);
	println(fast_io::out,fast_io::char_view(ch));
	int f(-1);
	println(fast_io::out,fast_io::unsigned_view(f),4,5,6);
	std::size_t v(-1);
	println(fast_io::out,fast_io::signed_view(v));
	std::size_t k(0);
	scan(fast_io::in,fast_io::signed_view(k));
	println(fast_io::out,k);
	scan(fast_io::in,fast_io::unsigned_view(f));
	println(fast_io::out,f);
}