#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<thread>
#include<random>
#include<vector>
#include<string_view>

std::string_view process_tag;

fast_io::sync_mutex file_log("log.txt",fast_io::open::interface<fast_io::open::out>);

void log_func(std::size_t thread_number)
{
	for(std::size_t i(0);i!=10000;++i)
		println_flush(file_log,"Process",fast_io::width<5>(process_tag)," Thread",fast_io::width<5>(thread_number)," Loop",fast_io::width<5>(i));
}

int main(int argc,char **argv)
{
	if(argc!=2)
	{
		print(fast_io::err,"Usage: ",*argv," <process tag>\n");
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