#include"../../include/fast_io.h"
#include<filesystem>

int main()
try
{
	fast_io::obuf process_log("process_log.txt");
	for(auto const & e : std::filesystem::directory_iterator("/proc/"))
	{
		auto stem(e.path().stem());
		auto string(stem.string());
		for(auto const & e : string)
			if(10<=static_cast<unsigned char>(e)-'0')
				goto nextloop;
		{
		println(process_log,"Process ID: ",string);
		{
		println(process_log,"\nStatus");
		fast_io::isystem_file ib(fast_io::concat<>("/proc/",string,"/status"));
		transmit(process_log,ib);
		println(process_log);
		}
		{
		println(process_log,"Maps");
		fast_io::isystem_file ib(fast_io::concat<>("/proc/",string,"/maps"));
		transmit(process_log,ib);
		}
		println(process_log,"\n");
		}
		nextloop:;
	}
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}
