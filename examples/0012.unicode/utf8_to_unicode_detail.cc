#include<iostream>
#include"../../include/fast_io.h"
#include<string_view>
int main()
{
	std::string_view vw("工要在地一，上是中国同和的有人我主产不为这经以发了民㍿dsgds\n");
	fast_io::obuf ob("utf8_to_unicode_detail.txt");
	for(auto const& e : vw )
		(ob<<e).put('\t');
	ob.put('\n');
	auto unc(fast_io::utf8_to_unicode<std::wstring>("工要在地一，上是中国同和的有人我主产不为这经以发了民㍿dsgds\n"));
	fast_io::out<<"wtee\n";
	for(auto const& e : unc )
		(ob<<e).put('\t');
	ob.put('\n');
	auto utf8(fast_io::unicode_to_utf8(unc));
	for(auto const& e : utf8 )
		(ob<<e).put('\t');
	ob.put('\n');
	(fast_io::out<<(vw==utf8)).put('\n');
	fast_io::out<<utf8;
}