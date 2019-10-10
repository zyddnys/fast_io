#include"../../include/fast_io.h"
#include"linux_apis.h"
#include<array>
#include<filesystem>

int main()
try
{
	using namespace std::string_view_literals;
	std::array<std::string_view,2> constexpr argv{"ps"sv,"-A"sv};
	fast_io::obuf process_log("process_log.txt");
	fast_io::native_stderr() = fast_io::system_file("process_error_log.txt",fast_io::open::interface<fast_io::open::out|fast_io::open::binary>);
	fast_io::pipe pipe;
	auto child_process{linux_apis::fork()};
	if(!child_process)		//this is child process
	{
		pipe.in().close();
		fast_io::native_stdout()=pipe.out();
		linux_apis::execve("/bin/ps",argv);
		return 0;
	}
	pipe.out().close();
	linux_apis::waitpid(child_process,nullptr,0);
	std::string data;
	getline(pipe,data);
	println(process_log,data);
	try
	{
		for(;;)
		{
			println(process_log);
			getline(pipe,data);
			println(process_log,data);
			fast_io::istring_view view(data);
			std::size_t process_id;
			scan(view,process_id);
			fast_io::pipe pipe_process_cat_map;
			auto child_process{linux_apis::fork()};
			if(!child_process)		//this is child process
			{
				pipe.in().close();
				pipe_process_cat_map.in().close();
				fast_io::native_stdout()=pipe_process_cat_map.out();
				std::array<std::string_view,2> const cat_argv{"cat"sv,fast_io::concat<>("/proc/",process_id,"/maps")};
				linux_apis::execve("/bin/cat",cat_argv);
				return 0;
			}
			pipe_process_cat_map.out().close();
			linux_apis::waitpid(child_process,nullptr,0);
			transmit(process_log,pipe_process_cat_map);
		}
	}
	catch(fast_io::eof const&){}
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}
