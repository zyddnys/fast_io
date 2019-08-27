#include"../../include/fast_io.h"

int main()
{
	fast_io::obuf obuf("file_hello_world.txt");
	println(obuf,"Hello World!");
}