#include"../../include/fast_io_core.h"

//Compilation success
//g++ -o linux_freestanding_number_output linux_freestanding_number_output.cc -O3 -std=c++2a -fconcepts -nostartfiles -ffreestanding -fno-exceptions -fno-rtti
//Execuation:./linux_freestanding_number_output; echo $?

extern "C"
{
void _start() noexcept
{
	std::array<char,10000> arr;
	fast_io::ospan out(arr);
	println(out,"asdhasdhidasogh120asfas\n");
	auto isv(to_istring_view(out));
	int value(5);
	scan(isv,value);
	_Exit(value);
}
}