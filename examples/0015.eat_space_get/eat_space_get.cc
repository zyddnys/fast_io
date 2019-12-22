#include"../../include/fast_io.h"

int main()
{
	fast_io::ibuf ib("sample.txt");
	put(fast_io::out,eat_space_get(ib));
	println(fast_io::out);
}