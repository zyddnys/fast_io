#include"timer.h"
#include"../include/fast_io.h"
#include<exception>
#include<thread>
#include<deque>
#include<condition_variable>
#include<vector>
#include<future>

struct my_thread:std::thread
{
	template<typename ...Args>
	my_thread(Args&& ...args):std::thread(std::forward<Args>(args)...){}
	~my_thread()
	{
		if(joinable())
			join();
	}
};
std::deque<std::size_t> dq;
std::mutex m;
std::condition_variable cv;
std::size_t num(0);
bool done(false);
void f()
{
	for(auto mynum(num++);done;)
	{
		std::unique_lock locker(m);
		cv.wait(locker,[]{return !done&&!dq.empty();});
		if(done&&dq.empty())
			break;
		std::size_t data(dq.back());
		dq.pop_back();
		locker.unlock();
		fprint(fast_io::out,"% got a value from producer: % \n",mynum,data);
	}
}

using namespace std::chrono_literals;
int main()
try
{
	cqw::timer t("total");
	std::size_t constexpr N(1000000);

	my_thread t1([&]()
	{
		for(std::size_t count(10);count;--count)
		{
			std::unique_lock locker(m);
			dq.push_front(count);
			locker.unlock();
			cv.notify_one();
			std::this_thread::sleep_for(1s);
		}
		done=true;
	});
	std::vector<std::future<void>> vec;
	for(std::size_t i(0),cores(std::thread::hardware_concurrency());i<cores;++i)
		vec.emplace_back(std::async(f));
	for(auto & e : vec)
		e.get();
}
catch(std::exception const& e)
{
	fast_io::log mlog;
	print(mlog,"%\n",e.what());
	return 1;
}