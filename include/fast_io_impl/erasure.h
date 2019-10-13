#pragma once

namespace fast_io
{
template<input_stream io>
class ierasure:public io
{
public:
	using char_type = typename io::char_type;
	template<typename ...Args>
	requires std::constructible_from<io,Args...>
	ierasure(Args&& ...args):io(std::forward<Args>(args)...){}
	template<std::contiguous_iterator Iter>
	Iter reads(Iter,Iter) = delete;
	auto get() requires standard_input_stream<io> = delete;
	auto try_get() requires standard_input_stream<io> = delete;
};

template<output_stream io>
class oerasure:public io
{
public:
	using char_type = typename io::char_type;
	template<typename ...Args>
	requires std::constructible_from<io,Args...>
	oerasure(Args&& ...args):io(std::forward<Args>(args)...){}
	template<std::contiguous_iterator Iter>
	void writes(Iter,Iter) = delete;
	auto put() requires standard_output_stream<io> = delete;
	auto flush() requires standard_output_stream<io> = delete;
};

template<random_access_stream io>
class seekerasure:public io
{
public:
	using char_type = typename io::char_type;
	template<typename ...Args>
	requires std::constructible_from<io,Args...>
	seekerasure(Args&& ...args):io(std::forward<Args>(args)...){}
	template<std::integral U>
	auto seek(U,seekdir)=delete;
	template<typename T,std::integral U> auto seek(seek_type_t<T>,U,seekdir) = delete;
};


}