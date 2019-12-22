#include"../../include/fast_io.h"
#include<thread>
#include<random>

std::size_t const random_process_number(std::random_device{}());

fast_io::osync_mutex file_log("log.txt");

void thread_function1()
{
	for(std::size_t i(0);i!=10000;++i)
		println(file_log,"process ",random_process_number," thread 1:",i);
}

void thread_function2()
{
	for(std::size_t i(0);i!=10000;++i)
		println(file_log,"process ",random_process_number," thread2:",i);
}

int main()
{
	std::thread th1(thread_function1);
	std::thread th2(thread_function2);
	for(std::size_t i(0);i!=10000;++i)
		println(file_log,"procecss ",random_process_number," main thread:",i);
	th1.join();
	th2.join();
}