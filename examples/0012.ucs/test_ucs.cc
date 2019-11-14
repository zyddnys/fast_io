#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<cassert>

int main()
{
	using namespace fast_io;
	fast_io::obuf ob("test_ucs.txt");
	for(char32_t i(0);i!=1000000;++i)
	{
		std::u32string str(1,i);
		assert(utf8_to_ucs<std::u32string>(ucs_to_utf8(str))==str);
	}
}