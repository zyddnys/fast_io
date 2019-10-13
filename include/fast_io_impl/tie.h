#pragma once
#include"concept.h"

namespace fast_io
{

template<stream T,output_stream out>
class tie
{
	out* o;
public:
	using native_handle_type = T;
	using char_type = typename native_handle_type::char_type;
private:
	T t;
public:
	template<typename ...Args>
	requires std::constructible_from<T,Args...>
	constexpr tie(out &oo,Args&& ...args):o(std::addressof(oo)),t(std::forward<Args>(args)...){}
	constexpr auto& to() {return *o;}
	constexpr auto& native_handle() {return t;}
	constexpr auto get() requires standard_input_stream<T>
	{
		o->flush();
		return t.get();
	}
	constexpr auto try_get() requires standard_input_stream<T>
	{
		o->flush();
		return t.try_get();
	}
	constexpr void put(char_type ch) requires standard_output_stream<T>
	{
		o->flush();
		return t.put(ch);
	}
	constexpr void flush() requires output_stream<T>
	{
		o->flush();
		t.flush();
	}
	template<std::contiguous_iterator Iter>
	constexpr Iter reads(Iter begin,Iter end) 	requires input_stream<T>
	{
		o->flush();
		return t.reads(begin,end);
	}
	template<std::contiguous_iterator Iter>
	constexpr void writes(Iter begin,Iter end) requires output_stream<T>
	{
		o->flush();
		return t.writes(begin,end);
	}
};

template<io_stream T>
class self_tie
{
public:
	using native_handle_type = T;
	using char_type = typename native_handle_type::char_type;
private:
	T t;
public:
	template<typename ...Args>
	requires std::constructible_from<T,Args...>
	constexpr self_tie(Args&& ...args):t(std::forward<Args>(args)...){}
	constexpr auto& native_handle() {return t;}
	constexpr auto get() requires standard_input_stream<T>
	{
		t.flush();
		return t.get();
	}
	constexpr auto try_get() requires standard_input_stream<T>
	{
		t.flush();
		return t.try_get();
	}
	constexpr void put(char_type ch) requires standard_output_stream<T>
	{
		return t.put(ch);
	}
	constexpr void flush() requires output_stream<T>
	{
		t.flush();
	}
	template<std::contiguous_iterator Iter>
	constexpr Iter reads(Iter begin,Iter end) 	requires input_stream<T>
	{
		t.flush();
		return t.reads(begin,end);
	}
	template<std::contiguous_iterator Iter>
	constexpr void writes(Iter begin,Iter end) requires output_stream<T>
	{
		return t.writes(begin,end);
	}
	template<typename... Args>
	auto seek(Args&& ...args) requires random_access_stream<native_handle_type>
	{
		t.flush();
		return t.seek(std::forward<Args>(args)...);
	}
};

}