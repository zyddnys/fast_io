#include"../../include/fast_io.h"

int main()
{
	using namespace fast_io;
	ibuf ib("asfaf.txt");
	seek(ib,4);
	char ch;
	scan(ib,char_view(ch));
	put(out,ch);
	put(out,'\n');
	seek(ib,seek_type<std::uint32_t>,-1,seekdir::end);
	scan(ib,char_view(ch));
	put(out,ch);
	put(out,'\n');
	seek(ib.std::size_t(-1));
}