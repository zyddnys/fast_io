#ifndef FAST_IO_RD_TYPE_H
#define FAST_IO_RD_TYPE_H
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

	standard_input_stream& operator>>(standard_input_stream& in,Unsigned_integer& a)
	{
		typename std::remove_reference<decltype(in)>::type::int_type ch;
		for(;!details::isdigit(ch=in.get()););
		for(a=ch-48;details::isdigit(ch=in.get());a=a*10+ch-48);
		return in;
	}
	
	standard_input_stream& operator>>(standard_input_stream& in,Signed_integer& a)
	{
		typename std::remove_reference<decltype(in)>::type::int_type ch;
		for(;!details::isdigit_or_minus(ch=in.get()););
		if(ch==45)
		{
			for(;details::isdigit(ch=in.get());a=a*10+ch-48);
			a=-a;
		}
		else
			for(a=ch-48;details::isdigit(ch=in.get());a=a*10+ch-48);
		return in;
	}
	standard_output_stream& operator<<(standard_output_stream& out,Unsigned_integer a)
	{
		if(a)
		{
			std::array<typename std::remove_reference<decltype(out)>::type::char_type,sizeof(a)*8> v;
			auto ed(v.data()+v.size());
			for(*--ed=a%10+48;a/=10;*--ed=a%10+48);
			out.write(ed,v.data()+v.size());
		}
		else
			out.put(48);
		return out;
	}
	output_stream& operator<<(output_stream& out,std::string_view str)
	{
		out.write(str.data(),str.data()+str.size());
		return out;
	}
	standard_input_stream& operator>>(standard_input_stream& in,std::string &str)
	{
		str.clear();
		decltype(in.get()) ch;
		for(;details::isspace(ch=in.get()););
		if(!in.eof())
			for(str.push_back(ch);!details::isspace(ch=in.get())&&!in.eof();str.push_back(ch));
		return in;
	}
	standard_output_stream& operator<<(standard_output_stream& out,Signed_integer a)
	{
		if(a)
		{
			if(a<0)
			{
				out.put('-');
				a=-a;
			}
			std::array<typename std::remove_reference<decltype(out)>::type::char_type,sizeof(a)*8> v;

			auto ed(v.data()+v.size());
			for(*--ed=a%10+48;a/=10;*--ed=a%10+48);
			out.write(ed,v.data()+v.size());
		}
		else
			out.put(48);
		return out;
	}

	namespace details
	{
		output_stream& fprint(output_stream &out,std::string_view format)
		{
			auto const percent_pos(format.find('%'));
			if(percent_pos!=std::string_view::npos)
				throw std::runtime_error("fprint() format error");
			out.write(format.data(),format.data()+format.size());
			return out;
		}
		template<typename T,typename ...Args>
		output_stream& fprint(output_stream &out,std::string_view format,T const& cr,Args&& ...args)
		{
			auto const percent_pos(format.find('%'));
			if(percent_pos==std::string_view::npos)
			{
				out.write(format.data(),format.data()+format.size());
				return out;
			}
			out.write(format.data(),format.data()+percent_pos);
			return fprint(out << cr,format.substr(percent_pos+1),std::forward<Args>(args)...);
		}
	}

	template<typename ...Args>
	output_stream& fprint(output_stream &out,std::string_view format,Args&& ...args)
	{
		return details::fprint(out,format,std::forward<Args>(args)...);
	}
	input_stream& scan(input_stream &in)
	{
		return in;
	}
	template<typename T,typename ...Args>
	input_stream& scan(input_stream &in,T& cr,Args&& ...args)
	{
		return scan(in>>cr,std::forward<Args>(args)...);
	}
	output_stream& print(output_stream &out)
	{
		return out;
	}
	template<typename T,typename ...Args>
	output_stream& print(output_stream &out,const T& cr,Args&& ...args)
	{
		return print(out<<cr,std::forward<Args>(args)...);
	}
}
#endif