#include"../../include/fast_io_device.h"

int main()
{
	fast_io::obuf_mutex obuf("file_hello_world.txt");
	println(obuf,"Hello World!");
}