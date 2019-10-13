#include<fstream>
#include"../../include/fast_io.h"

int main()
{
	static_assert(!fast_io::standard_output_stream<std::ofstream>,"ofstream should not be a standard_output_stream");
	static_assert(!fast_io::standard_input_stream<std::ifstream>,"ifstream should not be a standard_input_stream");
	fast_io::stream auto fin(fast_io::ibuf("abc.txt"));
//	gcc ICE: fast_io::stream auto fin1(std::ifstream("abc.txt"));
}