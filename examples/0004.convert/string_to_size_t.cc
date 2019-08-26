#include"../../include/fast_io.h"

int main()
{
	println(fast_io::out,fast_io::to<std::size_t>("w325d"));//ignore all none-number character
	println(fast_io::out,fast_io::to<std::size_t>("ddd"));//try to convert from an invalid string will throw exception
}