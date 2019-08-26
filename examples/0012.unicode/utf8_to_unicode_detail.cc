#include"../../include/fast_io.h"
#include<string_view>
int main()
{
	std::string_view vw("工要在地一，上是中国同和的有人我主产不为这经以发了民㍿dsgds\n");
	fast_io::obuf ob("utf8_to_unicode_detail.txt");
	for(auto const& e : vw )
		print(ob,fast_io::unsigned_view(e));
	println(ob);
	auto unc(fast_io::utf8_to_unicode<std::u32string>("工要在地一，上是中国同和的有人我主产不为这经以发了民㍿dsgds\n"));
	for(auto const& e : unc )
		(ob<<e).put('\t');
	println(ob);
}