#ifndef FAST_IO_MUTEX_H__
#define FAST_IO_MUTEX_H__
#include"concept.h"
#include<mutex>
#include<memory>
#include<string_view>
namespace fast_io
{

template<standard_input_stream T>
class basic_imutex
{
	std::unique_ptr<std::mutex> _mtx;
	T handler;
public:
	using input_handle_type = T;
	using char_type = typename T::char_type;
	using int_type = typename T::int_type;
	T& handle()
	{
		return handler;
	}
	std::mutex& mutex()
	{
		return *_mtx;
	}
	template<typename ...Args>
	basic_imutex(Args&& ...args):_mtx(std::make_unique<std::mutex>()),handler(std::forward<Args>(args)...){}
	Pointer read(Pointer begin,Pointer end)
	{
		std::lock_guard<std::mutex> lg(mutex());
		return handler.read(begin,end);
	}
	int_type get()
	{
		std::lock_guard<std::mutex> lg(mutex());
		return handler.get();
	}
	bool eof()
	{
		return handler.eof();
	}
	operator bool() const
	{
		return handler;
	}
};

mutex_input_stream& scan(mutex_input_stream &in)
{
	return in;
}
template<typename T,typename ...Args>
mutex_input_stream& scan(mutex_input_stream &in,T& cr,Args&& ...args)
{
	std::lock_guard lg(in.mutex());
	scan(in.handle(),cr,std::forward<Args>(args)...);
	return in;
}

template<typename ...Args>
mutex_input_stream& scan(mutex_input_stream &imtx,Args&& ...args)
{
	std::lock_guard lg(imtx.mutex());
	scan(imtx.handle(),std::forward<Args>(args)...);
	return imtx;
}

template<standard_output_stream T>
class basic_omutex
{
	std::unique_ptr<std::mutex> _mtx;
	T handler;
public:
	using output_handle_type = T;
	using char_type = typename T::char_type;
	using int_type = typename T::int_type;
	
	template<typename ...Args>
	basic_omutex(Args&& ...args):_mtx(std::make_unique<std::mutex>()),handler(std::forward<Args>(args)...){}
	T& handle()
	{
		return handler;
	}
	std::mutex& mutex()
	{
		return *_mtx;
	}
	template<Pointer p>
	void write(p begin,p end)
	{
		std::lock_guard lg(mutex());
		handler.write(begin,end);
	}
	void put(int_type ch)
	{
		std::lock_guard lg(mutex());
		return handler.put(ch);
	}
	void flush()
	{
		std::lock_guard lg(mutex());
		handler.flush();
	}
};
template<typename ...Args>
mutex_output_stream& print(mutex_output_stream &omtx,Args&& ...args)
{
	std::lock_guard lg(omtx.mutex());
	print(omtx.handle(),std::forward<Args>(args)...);
	return omtx;
}

template<typename ...Args>
mutex_output_stream& fprint(mutex_output_stream &omtx,std::string_view format,Args&& ...args)
{
	std::lock_guard lg(omtx.mutex());
	fprint(omtx.handle(),format,std::forward<Args>(args)...);
	return omtx;
}

template<typename ...Args>
mutex_output_stream& fprint(mutex_output_stream &omtx,Args&& ...args)
{
	std::lock_guard lg(omtx.mutex());
	fprint(omtx.handle(),std::forward<Args>(args)...);
	return omtx;
}
}
#endif