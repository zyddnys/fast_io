#include"../../include/fast_io.h"
#include<cstdint>

int main()
{
	fast_io::out<<"Please input 2 hex numbers\n";
	std::int64_t a,b;
	fast_io::in>>fast_io::hex(a)>>fast_io::hex(b);
	fprint(fast_io::out,"sum of %(hex:%)+%(hex:%) = %(hex:%)\n",a,fast_io::hex(a),b,fast_io::hex(b)
						,a+b,fast_io::hex(a+b));
}