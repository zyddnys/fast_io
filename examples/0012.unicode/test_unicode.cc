#include"../../include/fast_io.h"
#include<cassert>

int main()
{
	using namespace fast_io;
	fast_io::obuf ob("test_unicode.txt");
	for(char32_t i(0);i!=1000000;++i)
	{
		std::u32string str(1,i);
		assert(utf8_to_unicode<std::u32string>(unicode_to_utf8(str))==str);
	}
}