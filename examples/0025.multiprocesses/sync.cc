#include"../../include/fast_io.h"
#include<random>

int main()
{
	std::random_device rd;
	std::size_t process_random_number(rd());
	fast_io::osync sync("sync_log.txt");
	for(std::size_t i(0);i!=1000000;++i)
	{
		println(sync,"Process Random ",process_random_number," ",i);
		sync.flush();
	}
}