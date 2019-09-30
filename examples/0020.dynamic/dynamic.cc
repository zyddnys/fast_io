#include"../../include/fast_io.h"
#include"../../include/fast_io_impl/handlers/c_style.h"
#include<vector>
#include<functional>

int main()
{
	std::vector<fast_io::dynamic_standard_output_stream> vec;
	vec.emplace_back(std::in_place_type<fast_io::obuf>,"demo_to_file.txt");
	vec.emplace_back(std::in_place_type<fast_io::c_style_file>,"cstyle.txt","wb");
// no longer allowed since mutex stream is slow with locking every byte
//	vec.emplace_back(std::in_place_type<fast_io::obuf_mutex>,"demo_to_file_mutex.txt");

	for(auto & e : vec)
		println(e,"hello world",5,6,8);
}
