#pragma once
#include"concept.h"
#include<mutex>
#include<memory>
#include<string_view>
namespace fast_io
{

template<stream T>
class basic_iomutex
{
	std::unique_ptr<std::mutex> mtx;
	T handler;
public:
	using native_handle_type = T;
	using char_type = typename native_handle_type::char_type;
	template<typename ...Args>
	basic_iomutex(Args&& ...args):mtx(std::make_unique<std::mutex>()),handler(std::forward<Args>(args)...){}
	T& native_handle()
	{
		return handler;
	}
	std::mutex& mutex()
	{
		return *mtx;
	}
	template<typename ...Args>
	void write(Args&& ...args) requires output_stream<native_handle_type>()
	{
		std::lock_guard lg(mutex());
		handler.write(std::forward<Args>(args)...);
	}
	template<typename ...Args>
	void put(Args&& ...args) requires standard_output_stream<native_handle_type>()
	{
		std::lock_guard lg(mutex());
		handler.put(std::forward<Args>(args)...);
	}
	void flush() requires output_stream<native_handle_type>()
	{
		std::lock_guard lg(mutex());
		handler.flush();
	}
	template<typename Contiguous_iterator>
	Contiguous_iterator read(Contiguous_iterator begin,Contiguous_iterator end) requires input_stream<native_handle_type>()
	{
		std::lock_guard<std::mutex> lg(mutex());
		return handler.read(begin,end);
	}
	auto get() requires standard_input_stream<native_handle_type>()
	{
		std::lock_guard<std::mutex> lg(mutex());
		return handler.get();
	}
	auto try_get() requires standard_input_stream<native_handle_type>()
	{
		std::lock_guard<std::mutex> lg(mutex());
		return handler.try_get();
	}
	auto eof() requires standard_input_stream<native_handle_type>()
	{
		return handler.eof();
	}
};

template<typename ...Args>
inline mutex_output_stream& print(mutex_output_stream &omtx,Args&& ...args)
{
	std::lock_guard lg(omtx.mutex());
	print(omtx.native_handle(),std::forward<Args>(args)...);
	return omtx;
}

template<typename ...Args>
inline mutex_output_stream& fprint(mutex_output_stream &omtx,std::string_view format,Args&& ...args)
{
	std::lock_guard lg(omtx.mutex());
	fprint(omtx.native_handle(),format,std::forward<Args>(args)...);
	return omtx;
}

template<typename ...Args>
inline mutex_output_stream& fprint(mutex_output_stream &omtx,Args&& ...args)
{
	std::lock_guard lg(omtx.mutex());
	fprint(omtx.native_handle(),std::forward<Args>(args)...);
	return omtx;
}
}