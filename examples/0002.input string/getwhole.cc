#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::ibuf ib("getline.txt");
	std::string str;
	getwhole(ib,str);
	print(fast_io::out,str);
}