#include"../../include/fast_io.h"

int main()
{
	fast_io::ibuf ib("sample.txt");
	fast_io::out.put(eat_space_get(ib));
	println(fast_io::out);
}