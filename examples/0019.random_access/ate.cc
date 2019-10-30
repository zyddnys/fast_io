#include"../../include/fast_io.h"

int main()
{
	fast_io::ibuf ib("asfaf.txt",fast_io::open::interface<fast_io::open::ate>);
	put(fast_io::out,get(ib));
	println(fast_io::out);
}