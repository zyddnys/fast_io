#include"../../include/fast_io.h"
#include<limits>

int main()
{
	auto constexpr v(std::numeric_limits<std::int64_t>::max());
	print(fast_io::out,"minus\n");
	println(fast_io::out,fast_io::dec(-v));
	println(fast_io::out,fast_io::hex(-v));
	println(fast_io::out,fast_io::hexupper(-v));
	println(fast_io::out,fast_io::oct(-v));	
	println(fast_io::out,fast_io::bin(-v));	
	println(fast_io::out,fast_io::base<36>(-v));		//base 36. lowercase
	print(fast_io::out,"\nplus\n");
	println(fast_io::out,fast_io::dec(v));
	println(fast_io::out,fast_io::hex(v));
	println(fast_io::out,fast_io::hexupper(v));
	println(fast_io::out,fast_io::oct(v));	
	println(fast_io::out,fast_io::bin(v));
	println(fast_io::out,fast_io::base<36,true>(v));		//base 36. uppercase
}