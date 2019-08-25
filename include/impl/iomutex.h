#pragma once
#include"concept.h"
#include<mutex>
#include<memory>
#include<string_view>
namespace fast_io
{

template<standard_input_stream T>
class basic_imutex
{
	std::unique_ptr<std::mutex> mtx;
	T handler;
public:
	using native_handle_type = T;
	using char_type = typename native_handle_type::char_type;
	T& native_handle()
	{
		return handler;
	}
	std::mutex& mutex()
	{
		return *mtx;
	}
	template<typename ...Args>
	basic_imutex(Args&& ...args):mtx(std::make_unique<std::mutex>()),handler(std::forward<Args>(args)...){}
	template<typename Contiguous_iterator>
	Contiguous_iterator read(Contiguous_iterator begin,Contiguous_iterator end)
	{
		std::lock_guard<std::mutex> lg(mutex());
		return handler.read(begin,end);
	}
	auto get()
	{
		std::lock_guard<std::mutex> lg(mutex());
		return handler.get();
	}
	auto eof()
	{
		return handler.eof();
	}
};

inline mutex_input_stream& scan(mutex_input_stream &in)
{
	return in;
}
template<typename T,typename ...Args>
inline mutex_input_stream& scan(mutex_input_stream &in,T& cr,Args&& ...args)
{
	std::lock_guard lg(in.mutex());
	scan(in.native_handle(),cr,std::forward<Args>(args)...);
	return in;
}

template<typename ...Args>
inline mutex_input_stream& scan(mutex_input_stream &imtx,Args&& ...args)
{
	std::lock_guard lg(imtx.mutex());
	scan(imtx.native_handle(),std::forward<Args>(args)...);
	return imtx;
}

template<standard_output_stream T>
class basic_omutex
{
	std::unique_ptr<std::mutex> mtx;
	T handler;
public:
	using native_handle_type = T;
	using char_type = typename native_handle_type::char_type;
	template<typename ...Args>
	basic_omutex(Args&& ...args):mtx(std::make_unique<std::mutex>()),handler(std::forward<Args>(args)...){}
	T& native_handle()
	{
		return handler;
	}
	std::mutex& mutex()
	{
		return *mtx;
	}
	template<typename ...Args>
	void write(Args&& ...args)
	{
		std::lock_guard lg(mutex());
		handler.write(std::forward<Args>(args)...);
	}
	template<typename ...Args>
	void put(Args&& ...args)
	{
		std::lock_guard lg(mutex());
		handler.put(std::forward<Args>(args)...);
	}
	void flush()
	{
		std::lock_guard lg(mutex());
		handler.flush();
	}
};

template<standard_io_stream T>
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
	void write(Args&& ...args)
	{
		std::lock_guard lg(mutex());
		handler.write(std::forward<Args>(args)...);
	}
	template<typename ...Args>
	void put(Args&& ...args)
	{
		std::lock_guard lg(mutex());
		handler.put(std::forward<Args>(args)...);
	}
	void flush()
	{
		std::lock_guard lg(mutex());
		handler.flush();
	}
	template<typename Contiguous_iterator>
	Contiguous_iterator read(Contiguous_iterator begin,Contiguous_iterator end)
	{
		std::lock_guard<std::mutex> lg(mutex());
		return handler.read(begin,end);
	}
	auto get()
	{
		std::lock_guard<std::mutex> lg(mutex());
		return handler.get();
	}
	auto eof()
	{
		return handler.eof();
	}
	operator bool() const
	{
		return handler;
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