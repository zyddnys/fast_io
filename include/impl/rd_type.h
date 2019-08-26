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

template<char base,bool uppercase,standard_output_stream output>
inline output& output_base_number(output& out,Unsigned_integer a)
{
//number: 0:48 9:57
//upper: 65 :A 70: F
//lower: 97 :a 102 :f
	if(a)
	{
		std::array<typename output::char_type,sizeof(a)*8> v;
		auto ed(v.data()+v.size());
		do
		{
			if constexpr(10 < base)
			{
				--ed;
				auto temp(a%base);
				if(temp<10)
					*ed = temp+48;
				else
				{
					if constexpr (uppercase)
						*ed = temp+55;	
					else
						*ed = temp+87;
				}
			}
			else
				*--ed = a%base+48;
		}
		while(a/=base);
		out.write(ed,v.data()+v.size());
	}
	else
		out.put(48);
	return out;
}

template<char base,bool uppercase,Signed_integer T>
inline standard_output_stream& output_base_number(standard_output_stream& out,T a)
{
	if(a<0)
	{
		out.put('-');
		a=-a;
	}
	return output_base_number<base,uppercase>(out,static_cast<std::make_unsigned_t<T>>(a));
}

template<char base>
inline constexpr standard_input_stream& input_base_number(standard_input_stream& in,Unsigned_integer& a)
{
	auto constexpr baseed(48+base);
	while(true)
	{
		auto ch(in.get());
		if(48<=ch&&ch<=baseed)
		{
			a=ch-48;
			break;
		}
		else if constexpr (10 < base)
		{
			if(65<=ch&&ch<=65+base)
			{
				a=ch-55;
				break;
			}
			else if(97<=ch&&ch<=97+base)
			{
				a=ch-87;
				break;
			}
		}
	}
	while(true)
	{
		auto try_ch(in.try_get());
		if(48<=try_ch.first&&try_ch.first<=baseed)
			a=a*base+try_ch.first-48;
		else if constexpr (10 < base)
		{
			if(65<=try_ch.first&&try_ch.first<=65+base)
				a=a*base+try_ch.first-55;
			else if(97<=try_ch.first&&try_ch.first<=97+base)
				a=a*base+try_ch.first-87;
			else
				break;
		}
		else
			break;
	}
	return in;
}
template<char base>
inline constexpr standard_input_stream& input_base_number(standard_input_stream& in,Signed_integer& a)
{
	auto constexpr baseed(48+base);
	bool rev(false);
	while(true)
	{
		auto ch(in.get());
		if(48<=ch&&ch<=baseed)
		{
			a=ch-48;
			break;
		}
		else if(ch==45)
		{
			a=0;
			rev=true;
			break;
		}
		else if constexpr (10 < base)
		{
			if(65<=ch&&ch<=65+base)
			{
				a=ch-55;
				break;
			}
			else if(97<=ch&&ch<=97+base)
			{
				a=ch-87;
				break;
			}
		}
	}
	while(true)
	{
		auto try_ch(in.try_get());
		if(48<=try_ch.first&&try_ch.first<=baseed)
			a=a*base+try_ch.first-48;
		else if constexpr (10 < base)
		{
			if(65<=try_ch.first&&try_ch.first<=65+base)
				a=a*base+try_ch.first-55;
			else if(97<=try_ch.first&&try_ch.first<=97+base)
				a=a*base+try_ch.first-87;
			else
				break;
		}
		else
			break;
	}
	if(rev)
		a=-a;
	return in;
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

mutex_output_stream& operator<<(mutex_output_stream &omtx,auto const& args) = delete;
//Not allow to use operator << directly to mutex stream since it will cause performance issue. Use print/fprint instead

mutex_input_stream& operator>>(mutex_input_stream &omtx,auto& args) = delete;
//Not allow to use operator >> directly to mutex stream since it will cause performance issue. Use scan instead

template<typename ...Args>
inline constexpr output_stream& println(output_stream &out,Args&& ...args)
{
	print(out,std::forward<Args>(args)...).put('\n');
	return out;
}

}