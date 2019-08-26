#include"../../include/fast_io.h"

int main()
{
	fast_io::obuf_mutex obuf("file_hello_world.txt");
	print(obuf,"Hello World!\n","hw");
}