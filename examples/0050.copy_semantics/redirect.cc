#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
try
{
	fast_io::out=fast_io::osystem_file("file.txt");
	println(fast_io::out,"Hello World");
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}