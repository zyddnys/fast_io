#include"timer.h"
#include<fstream>
#include"../include/fast_io.h"
#include<exception>
#include<cmath>
#include<memory>
#include<cstdio>
#include"iomutex2.h"
#include<thread>

inline fast_io::log mlog;

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
int main()
try
{
	cqw::timer t("total");
	std::size_t constexpr N(1000000);
	fast_io::obuf_mutex omtx("mth.txt");
	my_thread t1([&]()
	{
		for(std::size_t i(N);i!=2*N;++i)
			fprint(omtx,"From f1: %\n",i);
	});
	my_thread t2([&]()
	{
		for(std::size_t i(2*N);i!=3*N;++i)
			fprint(omtx,"From f2: %\n",i);
	});
	for(std::size_t i(0);i!=N;++i)
		fprint(omtx,"from main: %\n",i);
}
catch(std::exception const& e)
{	
	print(mlog,"%\n",e.what());
	return 1;
}