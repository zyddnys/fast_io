#pragma once
#include"concept.h"
#include<cstddef>
#include<vector>

namespace fast_io
{

inline input_stream& read(input_stream& in,Trivial_copyable& v)
{
	auto address(std::addressof(v));
	if(in.read(address,address+1)!=(address+1))
		throw std::runtime_error("cannot read data from input_stream&");
	return in;
}

inline output_stream& write(output_stream& out,Trivial_copyable const& v)
{
	auto address(std::addressof(v));
	out.write(address,address+1);
	return out;
}

template<standard_input_stream input>
inline input& read_size(input& in,std::size_t& size)
{
	auto constexpr bytes(sizeof(in.get())*8);
	auto constexpr lshift(bytes-1);
	auto constexpr limit(static_cast<std::size_t>(1)<<lshift);
	auto constexpr limitm1(limit-1);
	for(std::size_t temp(0),lsf(0);lsf<sizeof(size)*8;lsf+=lshift)
	{
		std::make_unsigned_t<decltype(in.get())> ch(in.get());
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
	auto constexpr bytes(sizeof(typename output::char_type)*8);
	auto constexpr lshift(bytes-1);
	auto constexpr limit(static_cast<std::size_t>(1)<<lshift);
	auto constexpr limitm1(limit-1);
	for(;limitm1<size;size>>=lshift)
		out.put((size&limitm1)|limit);
	out.put(size);
	return out;
}

inline standard_input_stream& read(standard_input_stream& in,Dynamic_size_container& v)
{
	std::size_t size;
	read_size(in,size);
	v.resize(size);
	for(auto & e : v)
		read(in,e);
	return in;
}

inline standard_input_stream& read(standard_input_stream& in,Contiguous_trivial_dynamic_size_container& v)
{
	std::size_t size;
	read_size(in,size);
	v.resize(size);
	if(in.read(v.begin(),v.end())!=v.end())
		throw std::runtime_error("read contiguous trivial containers error");
	return in;
}

inline standard_output_stream& write(standard_output_stream& out,Contiguous_trivial_dynamic_size_container const& v)
{
	write_size(out,v.size());
	out.write(v.begin(),v.end());
	return out;
}

inline standard_output_stream& write(standard_output_stream& out,Dynamic_size_container const& v)
{
	write_size(out,v.size());
	for(auto const& e : v)
		write(out,e);
	return out;
}

inline standard_output_stream& write(standard_output_stream& out,Contiguous_fixed_size_none_trivial_copyable_container const& v)
{
	for(auto const& e : v)
		write(out,e);
	return out;
}


}