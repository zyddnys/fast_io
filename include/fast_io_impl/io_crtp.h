#pragma once

namespace fast_io
{

//https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
//Curiously recurring template pattern
//Maybe metaclasses proposal will fix this???

template<input_stream input>
requires (!io_stream<input>)
struct basic_out_crtp
{
};

template<input_stream input,std::contiguous_iterator Iter>
inline constexpr void writes(basic_out_crtp<input>& io,Iter cbegin,Iter cend)
{
	writes(static_cast<input&>(io).out(),cbegin,cend);
}

template<input_stream input>
requires requires(basic_out_crtp<input>& io,typename input::char_type ch)
{
	put(static_cast<input&>(io).out(),ch);
}
inline constexpr void put(basic_out_crtp<input>& io,typename input::char_type ch)
{
	put(static_cast<input&>(io).out(),ch);
}

template<output_stream output>
requires (!io_stream<output>)
struct basic_in_crtp
{
};

template<output_stream output,std::contiguous_iterator Iter>
inline constexpr auto reads(basic_in_crtp<output>& io,Iter begin,Iter end)
{
	return reads(static_cast<output&>(io).in(),begin,end);
}

template<output_stream output>
requires requires(basic_in_crtp<output>& io)
{
	get(static_cast<output&>(io).in());
}
inline constexpr auto get(basic_in_crtp<output>& io)
{
	return get(static_cast<output&>(io).in());
}

template<output_stream output>
requires requires(basic_in_crtp<output>& io)
{
	try_get(static_cast<output&>(io).in());
}
inline constexpr auto try_get(basic_in_crtp<output>& io)
{
	return try_get(static_cast<output&>(io).in());
}


}