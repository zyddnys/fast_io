#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

class A{};

int main()
{
	A a;
//A is a type not defined how to print. for iostream, the error messages will bloat your screen.
//fast_io is designed to generate only one error message
	println(fast_io::out,a);
}