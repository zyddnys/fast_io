#include"../../include/fast_io.h"

int main()
{
	using namespace fast_io::literals;
	fast_io::uint256_t v;
	fast_io::istring_view sv("cqwrteur");
	scan(sv,fast_io::base<36>(v));
//	("1abdc"_nh<<100000);
	
	println_flush(fast_io::out,fast_io::base<36>(v));
//	println(fast_io::out,"here: ",v," ",v.low," ",v.high);
//	fast_io::uint128_t v(3);
//	v+=fast_io::uint128_t(4);
}