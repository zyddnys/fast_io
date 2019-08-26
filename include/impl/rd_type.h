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
	inline bool isspace(Integral ch)
	{
		return ch==0x20||ch==0x0a||ch==0x0d||ch==0x09||ch==0x0b;
	}
	inline bool isdigit(Integral ch)
	{
		return 47<ch&&ch<58;
	}
	inline bool isdigit_or_minus(Integral ch)
	{
		return (47<ch&&ch<58)||(ch==45);
	}
}

template<standard_input_stream input>
inline input& operator>>(input& in,Unsigned_integer& a)
{
	decltype(in.get()) ch;
	while(!details::isdigit(ch=in.get()));
	a=ch-48;
	for(decltype(in.try_get()) tch;details::isdigit((tch=in.try_get()).first);a=a*10+ch-48);
	return in;
}

template<standard_input_stream input>
inline input& operator>>(input& in,Signed_integer& a)
{
	decltype(in.get()) ch;
	while(!details::isdigit_or_minus(ch=in.get()));
	if(ch==45)
	{
		a=0;
		for(decltype(in.try_get()) ch;details::isdigit((ch=in.try_get()).first);a=a*10+ch-48);
		a=-a;
	}
	else
	{
		a=ch-48;
		for(;details::isdigit((ch=in.try_get()).first);a=a*10+ch-48);
	}
	return in;
}

template<standard_output_stream output>
inline output& operator<<(output& out,Unsigned_integer a)
{
	if(a)
	{
		std::array<typename output::char_type,sizeof(a)*8> v;
		auto ed(v.data()+v.size());
		for(*--ed=a%10+48;a/=10;*--ed=a%10+48);
		out.write(ed,v.data()+v.size());
	}
	else
		out.put(48);
	return out;
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
	decltype(in.get()) ch;
	for(;details::isspace(ch=in.get()););
	str.clear();
	str.push_back(ch);
	for(decltype(in.try_get()) ch;!details::isspace((ch=in.try_get()).first);str.push_back(ch.first));
	return in;
}

template<standard_output_stream output>
inline output& operator<<(output& out,Signed_integer a)
{
	if(a)
	{
		if(a<0)
		{
			out.put('-');
			a=-a;
		}
		std::array<typename output::char_type,sizeof(a)*8> v;

		auto ed(v.data()+v.size());
		for(*--ed=a%10+48;a/=10;*--ed=a%10+48);
		out.write(ed,v.data()+v.size());
	}
	else
		out.put(48);
	return out;
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
inline output_stream& fprint(output_stream &out,std::string_view format,T const& cr,Args&& ...args)
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
	return fprint(out << cr,format,std::forward<Args>(args)...);
}
}

template<typename ...Args>
inline output_stream& fprint(output_stream &out,std::string_view format,Args&& ...args)
{
	return details::fprint(out,format,std::forward<Args>(args)...);
}

inline input_stream& scan(input_stream &in)
{
	return in;
}
template<typename T,typename ...Args>
inline input_stream& scan(input_stream &in,T& cr,Args&& ...args)
{
	return scan(in>>cr,std::forward<Args>(args)...);
}
inline output_stream& print(output_stream &out)
{
	return out;
}

template<typename T,typename ...Args>
inline output_stream& print(output_stream &out,T const& cr,Args&& ...args)
{
	return print(out<<cr,std::forward<Args>(args)...);
}

mutex_output_stream& operator<<(mutex_output_stream &omtx,auto const& args) = delete;
//Not allow to use operator << directly to mutex stream since it will cause performance issue. Use print/fprint instead

mutex_input_stream& operator>>(mutex_input_stream &omtx,auto& args) = delete;
//Not allow to use operator >> directly to mutex stream since it will cause performance issue. Use scan instead

}