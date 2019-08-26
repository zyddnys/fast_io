#include"../../include/fast_io.h"

int main()
{
	fast_io::obuf_mutex obuf("file_hello_world.txt");
	print(obuf,"Hello World!\n","hw");
//	obuf<<"hello world\n"; operator<< is not allowed for obuf_mutex since it would cause performance issue.
}