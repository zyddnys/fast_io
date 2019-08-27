#include"../../include/fast_io.h"

int main()
{
	using namespace fast_io;
	ibuf_mutex ib("asfaf.txt");
	ib.seek(4);
	println(out,char_view(ib.get()));
	ib.seek(seek_type<std::uint32_t>,-1,seekdir::end);
	println(out,char_view(ib.get()));
	ib.seek(std::size_t(-1));
}