#include"../../include/fast_io.h"

int main()
{
	fast_io::ibuf ib("sample.txt");
	println(fast_io::out,fast_io::character(eat_space_get(ib)));
}