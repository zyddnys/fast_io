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
};

template<input_stream io,std::contiguous_iterator Iter>
inline Iter reads(ierasure<io>&,Iter,Iter) = delete;
template<character_input_stream io>
inline auto get(ierasure<io>&) = delete;
template<character_input_stream io>
inline auto try_get(ierasure<io>&) = delete;

template<output_stream io>
class oerasure:public io
{
public:
	using char_type = typename io::char_type;
	template<typename ...Args>
	requires std::constructible_from<io,Args...>
	oerasure(Args&& ...args):io(std::forward<Args>(args)...){}
};

template<output_stream io,std::contiguous_iterator Iter>
inline auto writes(ierasure<io>&,Iter,Iter) = delete;
template<output_stream io>
inline auto flush(oerasure<io>&) = delete;
template<character_output_stream io>
inline auto put(oerasure<io>&,typename io::char_type) = delete;

}