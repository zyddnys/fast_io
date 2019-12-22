#include<fstream>
#include"../../include/fast_io.h"

int main()
{
	static_assert(!fast_io::character_output_stream<std::ofstream>,"ofstream should not be a character_output_stream");
	static_assert(!fast_io::character_input_stream<std::ifstream>,"ifstream should not be a character_input_stream");
//	gcc ICE: fast_io::stream auto fin1(std::ifstream("abc.txt"));
}