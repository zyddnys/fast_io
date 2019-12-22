#include"../../include/fast_io_device.h"
#include"../../include/fast_io.h"

int main()
try
{
	fast_io::obuf ob("owner.txt",fast_io::perms::owner_read);
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}