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
	using traits_type = typename native_interface_t::traits_type;
private:
	T t;
	using char_type = typename traits_type::char_type;
	using int_type = typename traits_type::int_type;
public:
	template<typename ...Args>
	tie(out &oo,Args&& ...args):o(oo),t(std::forward<Args>(args)...){}
	auto& to() {return o;}
	auto& native_handle() {return t;}
	auto get() requires standard_input_stream<T>()
	{
		o.flush();
		return t.get();
	}
	void put(int_type ch) requires standard_output_stream<T>()
	{
		o.flush();
		return t.put(ch);
	}
	void flush() requires output_stream<T>()
	{
		o.flush();
		t.flush();
	}
	bool eof() requires standard_input_stream<T>()
	{
		o.flush();
		return t.eof();
	}
	template<typename Contiguous_iterator>
	Contiguous_iterator read(Contiguous_iterator begin,Contiguous_iterator end) 	requires input_stream<T>()
	{
		o.flush();
		return t.read(begin,end);
	}
	template<typename Contiguous_iterator>
	void write(Contiguous_iterator begin,Contiguous_iterator end)	requires output_stream<T>()
	{
		o.flush();
		return t.write(begin,end);
	}
};

}