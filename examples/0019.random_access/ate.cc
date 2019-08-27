#include"../../include/fast_io.h"

int main()
{
	fast_io::ibuf ib("asfaf.txt",fast_io::open::interface<fast_io::open::ate>);
	fast_io::out.put(ib.get());
	println(fast_io::out);
}