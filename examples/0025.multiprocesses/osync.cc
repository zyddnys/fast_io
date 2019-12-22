#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<random>

int main()
{
	std::random_device rd;
	std::size_t process_random_number(rd());
	fast_io::sync sync("sync_log.txt");			//sync won't call flush in its destructor
	for(std::size_t i(0);i!=1000000;++i)
		println_flush(sync,"Process Random ",process_random_number," ",i);
}