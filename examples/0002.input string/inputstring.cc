#include"../../include/fast_io.h"

int main()
{
	fast_io::ibuf ib("getline.txt");
	std::string str;
	ib>>str;
	println(fast_io::out,str);
}