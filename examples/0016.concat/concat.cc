#include"../../include/fast_io.h"

int main()
{
	std::string str(fast_io::concat<>("fdb",5,"sdgsd","herj",6346));
	fast_io::println(fast_io::out,str);		// this means convert to std::string by default
	fast_io::print(fast_io::out,fast_io::concatln<>("fdb",5,"sdgsd","herj",6346));		// concatln
}