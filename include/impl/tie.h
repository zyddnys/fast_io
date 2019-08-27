#pragma once
#include"concept.h"

namespace fast_io
{

template<typename T,output_stream out>
class tie
{
	out& o;
public:
	using native_interface_t = T;
	using char_type = typename native_interface_t::char_type;
private:
	T t;
public:
	template<typename ...Args>
	constexpr tie(out &oo,Args&& ...args):o(oo),t(std::forward<Args>(args)...){}
	constexpr auto& to() {return o;}
	constexpr auto& native_handle() {return t;}
	constexpr auto get() requires standard_input_stream<T>
	{
		o.flush();
		return t.get();
	}
	constexpr auto try_get() requires standard_input_stream<T>
	{
		o.flush();
		return t.try_get();
	}
	constexpr void put(char_type ch) requires standard_output_stream<T>
	{
		o.flush();
		return t.put(ch);
	}
	constexpr void flush() requires output_stream<T>
	{
		o.flush();
		t.flush();
	}
	constexpr bool eof() requires standard_input_stream<T>
	{
		o.flush();
		return t.eof();
	}
	template<typename Contiguous_iterator>
	constexpr Contiguous_iterator read(Contiguous_iterator begin,Contiguous_iterator end) 	requires input_stream<T>
	{
		o.flush();
		return t.read(begin,end);
	}
	template<typename Contiguous_iterator>
	constexpr void write(Contiguous_iterator begin,Contiguous_iterator end) requires output_stream<T>
	{
		o.flush();
		return t.write(begin,end);
	}
};

}