#include"../../include/fast_io_legacy.h"

int main()
{
	fprintf(fast_io::c_stdout,"Hello %s\n","World");	// call fprintf
	println(fast_io::c_stdout,"Hello World");		// call fast_io's println
}