#include"../../include/fast_io.h"
#include<string_view>
int main()
{
	std::wstring_view vw(L"😀");
	fast_io::obuf ob("utf8_to_unicode_detail.txt");
	auto unc(fast_io::utf8_to_unicode<std::u16string>("😀"));
	for(auto const& e : vw )
		(ob<<e).put('\t');
	println(ob);
	for(auto const& e : unc )
		(ob<<e).put('\t');
	println(ob);
}