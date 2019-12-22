#ifndef TIMER_H
#define TIMER_H

#include<chrono>
#include<cstdio>
#include<string>

namespace cqw
{

class timer
{
	std::string s;
	std::chrono::high_resolution_clock::time_point t0;
public:
	template<typename ...Args>
	timer(Args&& ...args):s(std::forward<Args>(args)...),t0(std::chrono::high_resolution_clock::now()){}
	timer(const timer&) = delete;
	timer& operator=(const timer&) = delete;
	~timer()
	{
		fprintf(stderr,"%s:\t%.8fs\n",s.c_str(),std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now()-t0).count());
	}
};

}

#endif