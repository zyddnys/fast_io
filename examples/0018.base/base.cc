#include"../../include/fast_io.h"
#include<cstdint>

int main()
{
	std::int64_t constexpr value(1048576);
	print(fast_io::out,"minus\n");
	println(fast_io::out,fast_io::dec(-value));
	println(fast_io::out,fast_io::hex(-value));
	println(fast_io::out,fast_io::hexupper(-value));
	println(fast_io::out,fast_io::oct(-value));	
	println(fast_io::out,fast_io::bin(-value));	
	println(fast_io::out,fast_io::base<36>(-value));		//base 36. lowercase
	print(fast_io::out,"\nplus\n");
	println(fast_io::out,fast_io::dec(value));
	println(fast_io::out,fast_io::hex(value));
	println(fast_io::out,fast_io::hexupper(value));
	println(fast_io::out,fast_io::oct(value));	
	println(fast_io::out,fast_io::bin(value));
	println(fast_io::out,fast_io::base<36,true>(value));		//base 36. uppercase
}