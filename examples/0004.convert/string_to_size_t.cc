#include"../../include/fast_io.h"

int main()
try
{
	auto valid_number{fast_io::to<std::size_t>("w325d")};
	println(fast_io::out,"convert from string w325d: ",valid_number);//ignore all none-number character
	auto invalid_number{fast_io::to<std::size_t>("ddd")};//try to convert from an invalid string will throw exception
	println(fast_io::out,"convert from string ddd: ",invalid_number);
	println(fast_io::out,"no exception");
}
catch(std::exception const &e)
{
	println(fast_io::err,e);
	return 1;
}