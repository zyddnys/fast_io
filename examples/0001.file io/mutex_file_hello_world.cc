#include"../../include/fast_io.h"

int main()
{
	fast_io::obuf_mutex obuf("file_hello_world.txt");
//	obuf<<"hello world\n";		 This is forbidden for mutex stream! You can lock by yourself or use print/println/fprint etc
	print(obuf,"Hello World!\n","hw");
}