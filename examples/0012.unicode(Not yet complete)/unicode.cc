#include"../../include/fast_io.h"

int main()
{
	using namespace fast_io;
	std::string_view const emoji(u8"☭🚄😀我wubi😄😊😉😍😘😚😜😝😳😁😣😢😂😭😪😥😰😩㍿🀀🀁🀂🀃🀄🀅🀆🀇🀈🀉🀊🀋🀌🀍🀎🀏");
	for(auto const & e : emoji)
		print(out,bin(unsigned_view(e)),"\t");
	print(out,"\n\n");
	auto u32emoji(utf8_to_unicode<std::u32string>(emoji));
	println(out,"unicode characters: ",u32emoji.size());
	for(auto const & e : u32emoji)
	{
		std::u32string str(1,e);
		fprint(out,"% %(0x%)\n",unicode_to_utf8(str),bin(unsigned_view(e)),hexupper(unsigned_view(e)));
	}
	println(out);
	auto const convert_back(unicode_to_utf8(u32emoji));
	for(auto const& e : convert_back)
		print(out,bin(unsigned_view(e)),"\t");
	println(out,"\nSame?",emoji==convert_back);
}