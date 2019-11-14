#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<cstdlib>
#include<thread>
#include<vector>

void launch_process(std::size_t process_number)
{
	auto const ret(std::system(fast_io::concat<>("./multithread_sync ",process_number).c_str()));
}

int main()
{
	std::vector<std::thread> vecs;
	for(std::size_t i(0);i!=100;++i)
		vecs.emplace_back(launch_process,i);
	for(auto & e : vecs)
		e.join();
}