#pragma once
#include"concept.h"
#include<cstddef>

namespace fast_io
{
template<typename T>
requires std::is_trivially_copyable_v<T>
inline standard_input_stream& read(standard_input_stream& in,T& v)
{
	auto address(std::addressof(v));
	if(in.read(address,address+1)!=(address+1))
		throw std::runtime_error("cannot read data from standard_input_stream&");
	return in;
}

template<typename T>
requires std::is_trivially_copyable_v<T>
inline standard_output_stream& write(standard_output_stream& out,T const& v)
{
	auto address(std::addressof(v));
	out.write(address,address+1);
	return out;
}

template<standard_input_stream input>
inline input& read_size(input& in,std::size_t& size)
{
	auto constexpr bytes(sizeof(typename input::traits_type::char_type)*8);
	auto constexpr lshift(bytes-1);
	auto constexpr limit(static_cast<std::size_t>(1)<<lshift);
	auto constexpr limitm1(limit-1);
	typename input::traits_type::int_type ch;
	for(std::size_t temp(0),lsf(0);(ch=in.get())!=input::traits_type::eof()&&lsf<(sizeof(size)*8);lsf+=lshift)
	{
		temp|=(ch&limitm1)<<lsf;
		if(ch<limit)
		{
			size=temp;
			return in;
		}
	}
	throw std::runtime_error("incorrect read size");
}

template<standard_output_stream output>
inline output& write_size(output& out,std::size_t size)
{
	auto constexpr bytes(sizeof(typename output::traits_type::char_type)*8);
	auto constexpr lshift(bytes-1);
	auto constexpr limit(static_cast<std::size_t>(1)<<lshift);
	auto constexpr limitm1(limit-1);
	for(;limitm1<size;size>>=lshift)
		out.put((size&limitm1)|limit);
	out.put(size);
	return out;
}

}