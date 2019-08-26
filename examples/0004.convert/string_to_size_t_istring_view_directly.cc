#include"../../include/fast_io.h"

int main()
{
	fast_io::istring_view ivw("w325d ddd");
	std::size_t valid_number;
	ivw>>valid_number;
	println(fast_io::out,"convert from string w325d ddd: ",valid_number);//ignore all none-number character
	std::size_t invalid_number;
	ivw>>invalid_number;	//try to input from an invalid string_view will throw exception
	println(fast_io::out,"convert from string ddd: ",invalid_number);
	println(fast_io::out,"no exception");
}