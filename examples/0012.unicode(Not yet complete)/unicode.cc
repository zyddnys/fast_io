#include"../../include/fast_io.h"
#include<string_view>
int main()
{
//	fast_io::obuf ob("unicode.txt");
//	fast_io::unicode_view<decltype(ob),wchar_t> uv(ob);
//	println(uv,L"😀");
	std::string_view const emoji(u8"😀");
	for(auto const & e : emoji)
	{
		print(fast_io::out,fast_io::unsigned_view(e),"\t");
	}
}