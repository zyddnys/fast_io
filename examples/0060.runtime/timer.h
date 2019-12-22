#pragma once

#include<chrono>
#include"../../include/fast_io.h"
#include<string>

namespace cqw
{

class timer
{
	std::string_view s;
	std::chrono::high_resolution_clock::time_point t0;
public:
	template<typename ...Args>
	timer(Args&& ...args):s(std::forward<Args>(args)...),t0(std::chrono::high_resolution_clock::now()){}
	timer(const timer&) = delete;
	timer& operator=(const timer&) = delete;
	~timer()
	{
		println(fast_io::err,s,":\t",std::chrono::high_resolution_clock::now()-t0,fast_io::char_view('s'));
	}
};

}