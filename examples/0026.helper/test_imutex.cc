#include<iostream>
#include"../../include/fast_io.h"

int main()
{
	fast_io::ibuf_mutex ibuf_mutex("temp.txt");
//	std::cout<<fast_io::mutex_input_stream<fast_io::ibuf_mutex><<std::endl;
	std::size_t d;
	scan(ibuf_mutex,d);
	fast_io::obuf_mutex obuf_mutex("temp2.txt");
//	std::cout<<fast_io::mutex_input_stream<fast_io::ibuf_mutex><<std::endl;
	print(obuf_mutex,d);	
}