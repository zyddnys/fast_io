#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
try
{
	auto out_backup(fast_io::out);
	fast_io::out=fast_io::osystem_file("file.txt");
	println(fast_io::out,"Hello World");
	fast_io::out=out_backup;
	println(fast_io::out,"HA, I AM BACK HAHAHA. YOU FOOLISH STDIO.H AND IOSTREAM CANNOT EVEN RECOVER FROM FREOPEN");
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}