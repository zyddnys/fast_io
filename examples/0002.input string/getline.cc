#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::ibuf ib("getline.txt");
	std::string str;
	getline(ib,str);
	println(fast_io::out,str);
}