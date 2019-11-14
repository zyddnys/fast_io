#include"../../include/fast_io_device.h"

int main()
{
	std::size_t sz(23623643);
	std::string str(fast_io::to<std::string>(sz));
	println(fast_io::out,"convert from std::size_t 23623643: ",str);
}