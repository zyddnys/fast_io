#pragma once
#include<type_traits>
#include<array>
#include<stdexcept>
#include"concept.h"
#include"stringbuf.h"
#include<string_view>

namespace fast_io
{
namespace details
{
inline constexpr bool isspace(Integral ch)
{
	return ch==0x20||ch==0x0a||ch==0x0d||ch==0x09||ch==0x0b;
}
}

inline constexpr auto eat_space_get(standard_input_stream& in)
{
	decltype(in.get()) ch(in.get());
	for(;details::isspace(ch);ch=in.get());
	return ch;
}

template<standard_input_stream input>
inline void scan(input& in,Integral& a)
{
	details::input_base_number<10>(in,a);
}

template<standard_output_stream output>
inline constexpr void print(output& out,Integral a)
{
	details::output_base_number<10,false>(out,a);
}

template<output_stream output>
inline void print(output& out,std::basic_string_view<typename output::char_type> str)
{
	out.write(str.cbegin(),str.cend());
}

inline void print(output_stream& out,std::exception const &e)
{
	print(out,e.what());
}
template<standard_input_stream input>
inline void scan(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	str.push_back(eat_space_get(in));
	for(decltype(in.try_get()) ch;!(ch=in.try_get()).second&&!details::isspace(ch.first);str.push_back(ch.first));
}

template<standard_input_stream input>
inline void getline(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	for(decltype(in.try_get()) ch;!(ch=in.try_get()).second&&ch.first!='\n';str.push_back(ch.first));
}

template<standard_input_stream input>
inline void getwhole(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	for(decltype(in.try_get()) ch;!(ch=in.try_get()).second;str.push_back(ch.first));
}

namespace details
{
template<output_stream os,typename ...Args>
inline void fprint(os &out,std::basic_string_view<typename os::char_type> format)
{
	std::size_t percent_pos;
	for(;(percent_pos=format.find('%'))!=std::string_view::npos&&percent_pos+1!=format.size()&&format[percent_pos+1]=='%';format.remove_prefix(percent_pos+2))
		out.write(format.cbegin(),format.cbegin()+percent_pos+1);
	if(percent_pos!=std::string_view::npos)
		throw std::runtime_error("fprint() format error");
	out.write(format.cbegin(),format.cend());
}

template<output_stream os,typename T,typename ...Args>
inline void fprint(os &out,std::basic_string_view<typename os::char_type> format,T&& cr,Args&& ...args)
{
	std::size_t percent_pos;
	for(;(percent_pos=format.find('%'))!=std::string_view::npos&&percent_pos+1!=format.size()&&format[percent_pos+1]=='%';format.remove_prefix(percent_pos+2))
		out.write(format.cbegin(),format.cbegin()+percent_pos+1);
	if(percent_pos==std::string_view::npos)
	{
		out.write(format.cbegin(),format.cend());
		return;
	}
	else
	{
		out.write(format.cbegin(),format.cbegin()+percent_pos);
		format.remove_prefix(percent_pos+1);
	}
	print(out,std::forward<T>(cr));
	fprint(out,format,std::forward<Args>(args)...);
}
}

template<output_stream os,typename ...Args>
inline constexpr void fprint(os &out,std::basic_string_view<typename os::char_type> format,Args&& ...args)
{
	details::fprint(out,format,std::forward<Args>(args)...);
}


inline constexpr void scan(input_stream&){}

inline constexpr void print(output_stream&){}

template<typename T,typename R,typename ...Args>
inline constexpr void scan(input_stream &in,T&& ref,R&& ref1,Args&& ...args)
{
	scan(in,std::forward<T>(ref));
	scan(in,std::forward<R>(ref1));
	scan(in,std::forward<Args>(args)...);
}

template<typename T,typename R,typename ...Args>
inline constexpr void print(output_stream &out,T&&cr,R&&cr1,Args&& ...args)
{
	print(out,std::forward<T>(cr));
	print(out,std::forward<R>(cr1));
	print(out,std::forward<Args>(args)...);
}

template<typename ...Args>
inline constexpr void println(output_stream &out,Args&& ...args)
{
	print(out,std::forward<Args>(args)...);
	out.put('\n');
}

template<typename ...Args>
inline constexpr void print_flush(output_stream &out,Args&& ...args)
{
	print(out,std::forward<Args>(args)...);
	out.flush();
}

template<typename ...Args>
inline constexpr void println_flush(output_stream &out,Args&& ...args)
{
	println(out,std::forward<Args>(args)...);
	out.flush();
}

template<typename ...Args>
inline constexpr void fprint_flush(output_stream &out,Args&& ...args)
{
	fprint(out,std::forward<Args>(args)...);
	out.flush();
}

template<typename ...Args>
inline constexpr void write_flush(output_stream& out,Args&& ...args)
{
	write(out,std::forward<Args>(args)...);
	out.flush();
}

}