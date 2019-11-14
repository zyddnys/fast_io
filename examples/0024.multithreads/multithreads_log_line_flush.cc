#include"../../include/fast_io_device.h"
#include<thread>

fast_io::obuf_mutex file_log("log.txt");

void thread_function1()
{
	for(std::size_t i(0);i!=10000;++i)
		println_flush(file_log,"thread1:",i);
}

void thread_function2()
{
	for(std::size_t i(0);i!=10000;++i)
		println_flush(file_log,"thread2:",i);
}

int main()
{
	std::thread th1(thread_function1);
	std::thread th2(thread_function2);
	for(std::size_t i(0);i!=10000;++i)
		println_flush(file_log,"main thread:",i);
	th1.join();
	th2.join();
}