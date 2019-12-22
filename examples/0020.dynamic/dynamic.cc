#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<vector>
#include<functional>

int main()
try
{
	std::vector<fast_io::dynamic_stream> vec;
	vec.emplace_back(std::in_place_type<fast_io::obuf>,"demo_to_file.txt");
	vec.emplace_back(std::in_place_type<fast_io::system_io_handle>,fast_io::out);
	vec.emplace_back(std::in_place_type<fast_io::system_io_handle>,fast_io::native_stdout());
	vec.emplace_back(std::in_place_type<fast_io::system_io_handle>,fast_io::native_stderr());
	vec.emplace_back(std::in_place_type<fast_io::obuf_mutex>,"demo_to_file_mutex.txt");
	vec.emplace_back(fast_io::osystem_file("ok.txt"));
	for(auto & e : vec)
		println(e,"hello world",5,6,8);
	fast_io::dynamic_stream dyn(fast_io::ibuf("temp.txt"));
	println(dyn,"OH NO");//cannot print, should throw exception EPERM
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}