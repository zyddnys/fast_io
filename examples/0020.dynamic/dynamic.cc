#include"../../include/fast_io.h"
#include<vector>

int main()
{
	std::vector<fast_io::dynamic_standard_output_stream> vec;
	vec.emplace_back(std::in_place_type<fast_io::obuf>,"demo_to_file.txt");
	vec.emplace_back(std::in_place_type<fast_io::c_style_file>,"cstyle.txt","wb");
	vec.emplace_back(std::in_place_type<decltype(fast_io::out)>,fast_io::out);
	vec.emplace_back(std::in_place_type<fast_io::obuf_mutex>,"demo_to_file_mutex.txt");
	for(auto & e : vec)
		println(e,"hello world",5,6,8);
}