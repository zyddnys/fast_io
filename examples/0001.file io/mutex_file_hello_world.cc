#include"../../include/fast_io.h"

int main()
{
	fast_io::obuf_mutex obuf("file_hello_world.txt");// will automatically lock after each io operation. lock itself is exception safe
	obuf<<"Hello World!\n";
}