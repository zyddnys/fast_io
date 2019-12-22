#include"../../include/fast_io_device.h"

int main()
{
	fast_io::osystem_file ob("compile_time_open_mode.txt",fast_io::open::interface<fast_io::open::binary|fast_io::open::excl>);
}
