#include"../../include/fast_io.h"

int main()
{
	std::size_t v(5);
	fprint(fast_io::out,"address of v(%) is %(0x%)",v,fast_io::unsigned_view(std::addressof(v)),fast_io::hex(fast_io::unsigned_view(std::addressof(v))));
}