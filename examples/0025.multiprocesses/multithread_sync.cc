#include"../../include/fast_io.h"
#include<thread>
#include<random>
#include<vector>
#include<string_view>

std::string_view process_tag;

fast_io::osync_mutex file_log("log.txt",fast_io::open::interface<fast_io::open::out>);

void log_func(std::size_t thread_number)
{
	for(std::size_t i(0);i!=10000;++i)
	{
		println(file_log,"Process ",process_tag," Thread ",thread_number);
		file_log.flush();
	}
}

int main(int argc,char **argv)
{
	if(argc!=2)
	{
		println(fast_io::err,"Usage: ",*argv," <process tag>");
		return 1;
	}
	process_tag=argv[1];
	std::vector<std::thread> th;
	for(std::size_t i(1);i!=30;++i)
		th.emplace_back(log_func,i);
	log_func(0);
	for(auto & e : th)
		e.join();
}