#include"../../include/fast_io.h"

int main()
{
	using namespace fast_io;
	fast_io::obuf ob("test_unicode.txt");
	fast_io::unicode_view<decltype(ob),char32_t> view(ob);
	for(char32_t i(0);i!=200000;++i)
		println(view,i,char_view('\t'),char_view(i));
}