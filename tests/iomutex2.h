#ifndef FAST_IO_MUTEX2_H__
#define FAST_IO_MUTEX2_H__
namespace fast_io
{

template<standard_input_stream T>
class basic_imutex2
{
	std::mutex _mtx;
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
		return _mtx;
	}
	template<typename ...Args>
	basic_imutex2(Args&& ...args):handler(std::forward<Args>(args)...){}
	Pointer read(Pointer begin,Pointer end)
	{
		std::lock_guard<std::mutex> lg(_mtx);
		return handler.read(begin,end);
	}
	int_type get()
	{
		std::lock_guard<std::mutex> lg(_mtx);
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

template<standard_output_stream T>
class basic_omutex2
{
	std::mutex _mtx;
	T handler;
public:
	using output_handle_type = T;
	using char_type = typename T::char_type;
	using int_type = typename T::int_type;
	
	template<typename ...Args>
	basic_omutex2(Args&& ...args):handler(std::forward<Args>(args)...){}
	T& handle()
	{
		return handler;
	}
	std::mutex& mutex()
	{
		return _mtx;
	}
	template<Pointer p>
	void write(p begin,p end)
	{
		std::lock_guard<std::mutex> lg(_mtx);
		handler.write(begin,end);
	}
	void put(int_type ch)
	{
		std::lock_guard<std::mutex> lg(_mtx);
		return handler.put(ch);
	}
	void flush()
	{
		std::lock_guard<std::mutex> lg(_mtx);
		handler.flush();
	}
};

using ibuf_mutex2 = basic_imutex2<ibuf>;
using obuf_mutex2 = basic_omutex2<obuf>;

}
#endif