#include"../../include/fast_io.h"

int main()
{
	std::size_t sz(23623643);
	fast_io::ostring os;
	print(os,sz);
	std::string str(std::move(os.str()));			//YES. You can move string from ostring!
	println(fast_io::out,"convert from std::size_t 23623643: ",str);
}