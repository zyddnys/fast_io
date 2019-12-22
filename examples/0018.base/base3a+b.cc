#include"../../include/fast_io.h"
#include<cstdint>

int main()
{
	print(fast_io::out,"Please input 2 base3 numbers\n");
	std::int64_t a,b;
	scan(fast_io::in,fast_io::base<3>(a),fast_io::base<3>(b));
	fprint(fast_io::out,"sum of %([base3]:%)+%([base3]:%) = %([base3]:%)\n",a,fast_io::base<3>(a),b,fast_io::base<3>(b)
						,a+b,fast_io::base<3>(a+b));
}