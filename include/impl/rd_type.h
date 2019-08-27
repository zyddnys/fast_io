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
inline input& operator>>(input& in,Integral& a)
{
	return details::input_base_number<10>(in,a);
}

template<standard_output_stream output>
inline constexpr output& operator<<(output& out,Integral a)
{
	return details::output_base_number<10,false>(out,a);
}

template<output_stream output>
inline output& operator<<(output& out,std::basic_string_view<typename output::char_type> str)
{
	out.write(str.cbegin(),str.cend());
	return out;
}

inline output_stream& operator<<(output_stream& out,std::exception const &e)
{
	return out<<e.what();
}
template<standard_input_stream input>
inline input& operator>>(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	str.push_back(eat_space_get(in));
	for(decltype(in.try_get()) ch;!details::isspace((ch=in.try_get()).first);str.push_back(ch.first));
	return in;
}

template<standard_input_stream input>
inline input& getline(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	for(decltype(in.try_get()) ch;!(ch=in.try_get()).second&&ch.first!='\n';str.push_back(ch.first));
	return in;
}

template<standard_input_stream input>
inline input& getwhole(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	for(decltype(in.try_get()) ch;!(ch=in.try_get()).second;str.push_back(ch.first));
	return in;
}

namespace details
{
inline output_stream& fprint(output_stream &out,std::string_view format)
{
	std::size_t percent_pos;
	for(;(percent_pos=format.find('%'))!=std::string_view::npos&&percent_pos+1!=format.size()&&format[percent_pos+1]=='%';format.remove_prefix(percent_pos+2))
		out.write(format.cbegin(),format.cbegin()+percent_pos+1);
	if(percent_pos!=std::string_view::npos)
		throw std::runtime_error("fprint() format error");
	out.write(format.cbegin(),format.cend());
	return out;
}
template<typename T,typename ...Args>
inline output_stream& fprint(output_stream &out,std::string_view format,T&& cr,Args&& ...args)
{
	std::size_t percent_pos;
	for(;(percent_pos=format.find('%'))!=std::string_view::npos&&percent_pos+1!=format.size()&&format[percent_pos+1]=='%';format.remove_prefix(percent_pos+2))
		out.write(format.cbegin(),format.cbegin()+percent_pos+1);
	if(percent_pos==std::string_view::npos)
	{
		out.write(format.cbegin(),format.cend());
		return out;
	}
	else
	{
		out.write(format.cbegin(),format.cbegin()+percent_pos);
		format.remove_prefix(percent_pos+1);
	}
	return fprint(out << std::forward<T>(cr),format,std::forward<Args>(args)...);
}
}

template<typename ...Args>
inline constexpr output_stream& fprint(output_stream &out,std::string_view format,Args&& ...args)
{
	return details::fprint(out,format,std::forward<Args>(args)...);
}

inline constexpr input_stream& scan(input_stream &in)
{
	return in;
}

template<typename T,typename ...Args>
inline constexpr input_stream& scan(input_stream &in,T&& cr,Args&& ...args)
{
	return scan(in>>std::forward<T>(cr),std::forward<Args>(args)...);
}

inline constexpr output_stream& print(output_stream &out)
{
	return out;
}

template<typename T,typename ...Args>
inline constexpr output_stream& print(output_stream &out,T const& cr,Args&& ...args)
{
	return print(out<<cr,std::forward<Args>(args)...);
}

template<typename ...Args>
inline constexpr output_stream& println(output_stream &out,Args&& ...args)
{
	print(out,std::forward<Args>(args)...).put('\n');
	return out;
}

}