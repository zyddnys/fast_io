#include"../../include/fast_io.h"

int main()
{
	fast_io::ibuf_mutex ib("asfaf.txt");
	ib.seek(4);
	fast_io::println(fast_io::out,fast_io::char_view(ib.get()));
	ib.seek(fast_io::seek_type<std::uint32_t>,-1,fast_io::seekdir::end);
	fast_io::println(fast_io::out,fast_io::char_view(ib.get()));
	ib.seek(std::size_t(-1));
}