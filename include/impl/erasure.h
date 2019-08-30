#pragma once
#include"concept.h"

namespace fast_io
{
template<input_stream io>
class ierasure:public io
{
public:
	using char_type = typename io::char_type;
	template<typename ...Args>
	ierasure(Args&& ...args):io(std::forward<Args>(args)...){}
	template<typename Contiguous_iterator>
	Contiguous_iterator read(Contiguous_iterator begin,Contiguous_iterator end) = delete;
	auto get() requires standard_input_stream<io> = delete;
	auto try_get() requires standard_input_stream<io> = delete;
};

template<output_stream io>
class oerasure:public io
{
public:
	using char_type = typename io::char_type;
	template<typename ...Args>
	oerasure(Args&& ...args):io(std::forward<Args>(args)...){}
	template<typename Contiguous_iterator>
	void write(Contiguous_iterator begin,Contiguous_iterator end) = delete;
	auto put() requires standard_output_stream<io> = delete;
	auto flush() requires standard_output_stream<io> = delete;
};

template<random_access_stream io>
class seekerasure:public io
{
public:
	using char_type = typename io::char_type;
	template<typename ...Args>
	seekerasure(Args&& ...args):io(std::forward<Args>(args)...){}
	void seek(Integral,seekdir)=delete;
	template<typename T> void seek(seek_type_t<T>,Integral,seekdir) = delete;
};


}