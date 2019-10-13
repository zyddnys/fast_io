#pragma once

namespace fast_io
{
template<input_stream input>
inline constexpr void read(input&){}
template<output_stream output>
inline constexpr void write(output&){}

template<input_stream input,Trivial_copyable T>
inline void read(input& in,T& v)
{
	auto address(std::addressof(v));
	if(in.reads(address,address+1)!=(address+1))
		throw std::runtime_error("cannot read data from input_stream&");
}

template<output_stream output,Trivial_copyable T>
inline void write(output& out,T const& v)
{
	auto address(std::addressof(v));
	out.writes(address,address+1);
}

template<standard_input_stream input>
inline std::size_t read_size(input& in)
{
	auto constexpr bytes(sizeof(in.get())*8);
	auto constexpr lshift(bytes-1);
	auto constexpr limit(static_cast<std::size_t>(1)<<lshift);
	auto constexpr limitm1(limit-1);
	for(std::size_t temp(0),lsf(0);lsf<sizeof(std::size_t)*8;lsf+=lshift)
	{
		std::make_unsigned_t<decltype(in.get())> ch(in.get());
		temp|=(ch&limitm1)<<lsf;
		if(ch<limit)
			return temp;
	}
	throw std::runtime_error("size is out of std::size_t range");
}

template<standard_output_stream output>
inline void write_size(output& out,std::size_t size)
{
	using ch_type = typename output::char_type;
	auto constexpr bytes(sizeof(typename output::char_type)*8);
	auto constexpr lshift(bytes-1);
	auto constexpr limit(static_cast<std::size_t>(1)<<lshift);
	auto constexpr limitm1(limit-1);
	for(;limitm1<size;size>>=lshift)
		out.put(static_cast<ch_type>((size&limitm1)|limit));
	out.put(static_cast<ch_type>(size));
}

template<standard_input_stream input,Dynamic_size_container D>
inline void read(input& in,D& v)
{
	v.resize(read_size(in));
	for(auto & e : v)
		read(in,e);
}

template<standard_input_stream input,Contiguous_trivial_dynamic_size_container D>
inline void read(input& in,D& v)
{
	v.resize(read_size(in));
	if(in.reads(v.begin(),v.end())!=v.end())
		throw std::runtime_error("read contiguous trivial containers error");
}

template<standard_output_stream output,Contiguous_trivial_dynamic_size_container D>
inline void write(output& out,D const& v)
{
	write_size(out,v.size());
	out.writes(v.begin(),v.end());
}

template<standard_output_stream output,Dynamic_size_container D>
inline void write(output& out,D const& v)
{
	write_size(out,v.size());
	for(auto const& e : v)
		write(out,e);
}

template<standard_output_stream output,Contiguous_fixed_size_none_trivial_copyable_container D>
inline void write(output& out,D const& v)
{
	for(auto const& e : v)
		write(out,e);
}

template<output_stream output,typename T,typename R,typename ...Args>
inline constexpr void write(output& out,T&& t,R&& r,Args&& ...args)
{
	write(out,std::forward<T>(t));
	write(out,std::forward<R>(r));
	write(out,std::forward<Args>(args)...);
}

template<input_stream input,typename T,typename R,typename ...Args>
inline constexpr void read(input& in,T&& t,R&& r,Args&& ...args)
{
	read(in,std::forward<T>(t));
	read(in,std::forward<R>(r));
	read(in,std::forward<Args>(args)...);
}

}