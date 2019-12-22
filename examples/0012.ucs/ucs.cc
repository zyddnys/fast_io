#include"../../include/fast_io_device.h"

int main()
{
	fast_io::ucs<fast_io::obuf,char32_t> view("test_ucs.txt");
	for(char32_t i(0);i!=200000;++i)
		println(view,i,fast_io::char_view('\t'),fast_io::char_view(i));
}