#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<vector>
#include<functional>

int main()
{
	std::vector<fast_io::dynamic_output_stream> vec;
	vec.emplace_back(std::in_place_type<fast_io::obuf>,"demo_to_file.txt");
	vec.emplace_back(std::in_place_type<fast_io::io_ref<decltype(fast_io::out)>>,fast_io::out);
// no longer allowed since mutex stream is slow with locking every byte
//	vec.emplace_back(std::in_place_type<fast_io::obuf_mutex>,"demo_to_file_mutex.txt");

	for(auto & e : vec)
		println(e,"hello world",5,6,8);
//	static_assert(fast_io::output_stream<fast_io::dynamic_output_stream>,"sdgdsg");
}
