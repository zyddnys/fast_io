#include"../../include/fast_io.h"

int main()
{
	fast_io::obuf ob("compile_time_open_mode.txt",fast_io::open::interface<fast_io::open::binary|fast_io::open::excl>);
}
