#include"../../include/fast_io.h"

int main()
{
	using namespace fast_io;
	ibuf ib("asfaf.txt");
	ib.seek(4);
	char ch;
	scan(ib,char_view(ch));
	out.put(ch);
	out.put('\n');
	ib.seek(seek_type<std::uint32_t>,-1,seekdir::end);
	scan(ib,char_view(ch));
	out.put(ch);
	out.put('\n');
	ib.seek(std::size_t(-1));
}