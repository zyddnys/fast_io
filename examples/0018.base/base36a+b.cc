#include"../../include/fast_io.h"
#include<cstdint>

int main()
{
	print(fast_io::out,"Please input 2 base 36 numbers\n");
	std::int64_t a,b;
	scan(fast_io::in,fast_io::base<36>(a),fast_io::base<36>(b));
	fprint(fast_io::out,"sum of %([base36]:%)+%([base36]:%) = %([base36]:%)\n",a,fast_io::base<36>(a),b,fast_io::base<36>(b)
						,a+b,fast_io::base<36>(a+b));
}