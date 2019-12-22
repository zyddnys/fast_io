#include"../../include/fast_io.h"
#include<cstdint>

int main()
{
	print(fast_io::out,"Please input 2 base 25 numbers\n");
	std::int64_t a,b;
	scan(fast_io::in,fast_io::base<25>(a),fast_io::base<25>(b));
	fprint(fast_io::out,"sum of %([base25]:%)+%([base25]:%) = %([base25]:%)\n",a,fast_io::base<25>(a),b,fast_io::base<25>(b)
						,a+b,fast_io::base<25>(a+b));
}