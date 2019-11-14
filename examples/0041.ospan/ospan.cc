#include"../../include/fast_io.h"

int main()
{
	std::array<char,10000> arr;
	fast_io::ospan osp(arr);
	println(osp,354);
	print(fast_io::out,osp);
}