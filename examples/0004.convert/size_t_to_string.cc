#include"../../include/fast_io.h"

int main()
{
	std::string str(fast_io::to<std::string>("23623643"));
	println(fast_io::out,"convert from number 23623643: ",str);
}