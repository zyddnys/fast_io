#include"../../include/fast_io.h"
#include<cstdlib>
#include<thread>
#include<vector>

void launch_process(std::size_t process_number)
{
	std::system(fast_io::concat<>(".\\process_number ",process_number).c_str());
}

int main()
{
	std::vector<std::thread> vecs;
	for(std::size_t i(0);i!=100;++i)
		vecs.emplace_back(launch_process,i);
	for(auto & e : vecs)
		e.join();
}