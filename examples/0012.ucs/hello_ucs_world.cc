#include"../../include/fast_io.h"

int main()
{
	fast_io::ucs<decltype(fast_io::out),wchar_t> view(fast_io::out);
	print(view,L"Hello ucs World! I am print! My number is ",0,fast_io::char_view('\n'));
	println(view,L"Hello ucs World! I am println! My number is ",1);
	fprint(view,L"Hello ucs World! I am fprint! My number is %\n",2);
	print_flush(view,L"Hello ucs World! I am print_flush! My number is ",3,fast_io::char_view('\n'));
	println_flush(view,L"Hello ucs World! I am println_flush! My number is ",4);
	fprint_flush(view,L"Hello ucs World! I am fprint_flush! My number is %\n",5);
}