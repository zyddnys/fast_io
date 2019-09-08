#pragma once

namespace fast_io
{
inline constexpr void read(input_stream&){}
inline constexpr void write(output_stream&){}

inline void read(input_stream& in,Trivial_copyable& v)
{
	auto address(std::addressof(v));
	if(in.read(address,address+1)!=(address+1))
		throw std::runtime_error("cannot read data from input_stream&");
}

inline void write(output_stream& out,Trivial_copyable const& v)
{
	auto address(std::addressof(v));
	out.write(address,address+1);
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
	auto constexpr bytes(sizeof(typename output::char_type)*8);
	auto constexpr lshift(bytes-1);
	auto constexpr limit(static_cast<std::size_t>(1)<<lshift);
	auto constexpr limitm1(limit-1);
	for(;limitm1<size;size>>=lshift)
		out.put((size&limitm1)|limit);
	out.put(size);
}

inline void read(standard_input_stream& in,Dynamic_size_container& v)
{
	v.resize(read_size(in));
	for(auto & e : v)
		read(in,e);
}

inline void read(standard_input_stream& in,Contiguous_trivial_dynamic_size_container& v)
{
	v.resize(read_size(in));
	if(in.read(v.begin(),v.end())!=v.end())
		throw std::runtime_error("read contiguous trivial containers error");
}

inline void write(standard_output_stream& out,Contiguous_trivial_dynamic_size_container const& v)
{
	write_size(out,v.size());
	out.write(v.begin(),v.end());
}

inline void write(standard_output_stream& out,Dynamic_size_container const& v)
{
	write_size(out,v.size());
	for(auto const& e : v)
		write(out,e);
}

inline void write(standard_output_stream& out,Contiguous_fixed_size_none_trivial_copyable_container const& v)
{
	for(auto const& e : v)
		write(out,e);
}

template<typename T,typename R,typename ...Args>
inline constexpr void write(output_stream& out,T&& t,R&& r,Args&& ...args)
{
	write(out,std::forward<T>(t));
	write(out,std::forward<R>(r));
	write(out,std::forward<Args>(args)...);
}

template<typename T,typename R,typename ...Args>
inline constexpr void read(input_stream& in,T&& t,R&& r,Args&& ...args)
{
	read(in,std::forward<T>(t));
	read(in,std::forward<R>(r));
	read(in,std::forward<Args>(args)...);
}

}