#include"../../include/fast_io_device.h"

int main()
{
	fast_io::obuf ob("compile_time_open_mode.txt",fast_io::open::binary|fast_io::open::direct,fast_io::perms::owner_write);
}
