#include<concepts>
#include<cstdint>
#include<cstddef>
#include<cmath>
#include"../../include/fast_io.h"

int main()
{
	using namespace fast_io::literals;
	fast_io::natural a("12342352364363462436432624362358129402424248"_n);
	for(std::size_t i(0);i<2;++i)
	a*=a;
//	a-=fast_io::natural("3342352364363462436432624362358129408"_n);
	//println(fast_io::out,a);

	auto d1("12342352364363462436432624362358129402424248"_n), d2("12342352364363462436432624362358129402424248"_n), mod("11451419198103777777777777"_n);
	mod *= mod;
	//println(fast_io::out,"mod:",mod);
	auto ret = pow_mod(d1, d2, mod);
	
	println(fast_io::out,"ret:",ret);

}