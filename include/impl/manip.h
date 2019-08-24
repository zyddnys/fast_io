#ifndef FAST_IO_MANIP_H
#define FAST_IO_MANIP_H

#include"concept.h"
#include"stringbuf.h"
#include"rd_type.h"
#include<type_traits>
#include<cmath>

namespace fast_io
{
	namespace details
	{
		template<typename T>
		class const_character_t
		{
		public:
			const T& reference;
			const_character_t(const T &_t):reference(_t){}			
		};
		template<typename T>
		class character_t
		{
		public:
			T& reference;
			character_t(T& _t):reference(_t){}	
		};
		
		template<typename T>
		class setw_t
		{
		public:
			const std::size_t width;
			const T& reference;
			setw_t(std::size_t _w,const T& _t):width(_w),reference(_t){}
		};
		template<typename T,Integral char_type>
		class setw_fill_t
		{
		public:
			const std::size_t width;
			const T& reference;
			const char_type ch;
			setw_fill_t(std::size_t _w,const T& _t,char_type _ch):width(_w),reference(_t),ch(_ch){}
		};
		

		struct fixed
		{
		public:
			double const& reference;
			std::size_t const precision;
		};
		struct scientific
		{
		public:
			double const& reference;
			std::size_t const precision;
		};
		struct floating_point_default
		{
		public:
			double const& reference;
			std::size_t const precision;
		};
		class flush_t
		{};
	}
	template<Integral T>
	details::character_t<T> character(T &ch)
	{
		return details::character_t<T>(ch);
	}
	template<Integral T>
	details::const_character_t<T> character(const T &ch)
	{
		return details::const_character_t<T>(ch);
	}
	
//	template<template T>
//	requires requires std::is_floating_point_v<T>
	inline details::fixed fixed(double const &f,std::size_t precision)
	{
		return {f,precision};
	}
	inline details::scientific scientific(double const &f,std::size_t precision)
	{
		return {f,precision};
	}
	inline details::floating_point_default floating_point_default(double const &f,std::size_t precision)
	{
		return {f,precision};
	}
	standard_input_stream& operator>>(standard_input_stream& in,details::character_t<Integral> &a)
	{
		while(details::isspace(a.reference = in.get()));
		return in;
	}
	standard_output_stream& operator<<(standard_output_stream& out,const details::const_character_t<Integral> &a)
	{
		out.put(a.reference);
		return out;
	}
	standard_output_stream& operator<<(standard_output_stream& out,const details::character_t<Integral> &a)
	{
		out.put(a.reference);
		return out;
	}

	details::flush_t flush;
	standard_output_stream& operator<<(standard_output_stream& out,details::flush_t)
	{
		out.flush();
		return out;
	}

	template<typename T>
	details::setw_t<T> setw(std::size_t width,const T &t)
	{
		return details::setw_t<T>(width,t);
	}
	template<typename T>
	details::setw_fill_t<T,Integral> setw(std::size_t width,const T &t,Integral ch)
	{
		return details::setw_fill_t<T,decltype(ch)>(width,t,ch);
	}
	template<standard_output_stream output>
	output& operator<<(output& out,const details::setw_fill_t<auto,Integral> &a)
	{
		basic_obuf_string<std::basic_string<typename output::traits_type::char_type>> bas;
		bas<<a.reference;
		for(std::size_t i(bas.str().size());i<a.width;++i)
			out.put(a.ch);
		return out<<bas.str();
	}
	template<standard_output_stream output>
	output& operator<<(output& out,const details::setw_t<auto> &a)
	{
		basic_obuf_string<std::basic_string<typename output::traits_type::char_type>> bas;
		bas<<a.reference;
		for(std::size_t i(bas.str().size());i<a.width;++i)
			out.put(' ');
		return out<<bas.str();
	}
	
namespace details
{
template<std::size_t v>
inline auto constexpr compiler_time_10_exp_calculation()
{
	std::array<std::uint64_t,v> a{1};
	for(std::size_t i(1);i<a.size();++i)
		a[i]=a[i-1]*10;
	return a;
}

inline auto constexpr exp10_array(compiler_time_10_exp_calculation<20>());
inline auto constexpr log2_minus_10(-std::log2(10));

}

standard_output_stream& operator<<(standard_output_stream& out,details::fixed const &a)
{
	auto e(a.reference);
	if(e<0)
	{
		e=-e;
		out.put('-');
	}
	std::uint_fast64_t u(e);
	out<<u;
	e-=u;
	if(a.precision)
	{
		out.put('.');
		auto p(static_cast<decltype(u)>(e*details::exp10_array.at(a.precision+1)));
		auto md(p%10),pt(p/10);
		if(md<5||(md==5&&!(pt&1)))
			out<<setw(a.precision,pt,'0');
		else
			out<<setw(a.precision,pt+1,'0');
	}
	return out;
}

standard_output_stream& operator<<(standard_output_stream& out,details::scientific const &a)
{
	auto e(a.reference);
	if(e<0)
	{
		e=-e;
		out.put('-');
	}
	auto x(std::floor(std::log10(e)));
	if(0<x)
		++x;
	out<<fixed(e*std::exp2(x*details::log2_minus_10),a.precision);
	if(x==0)
		return out;
	out.put('e');
	if(x<0)
	{
		out.put('-');
		return out<<static_cast<std::uint64_t>(-x);
	}
	return out<<static_cast<std::uint64_t>(x);
}

template<standard_output_stream output>
output& operator<<(output& out,details::floating_point_default const &a)
{
	auto e(a.reference);
	if(e<0)
	{
		e=-e;
		out.put('-');
	}
	auto x(std::floor(std::log10(e)));
	if(0<x)
		++x;
	{
	auto fix(std::fabs(x)<=a.precision);
	basic_obuf_string<std::basic_string<typename output::traits_type::char_type>> bas;
	if(fix)
		bas<<fixed(e,a.precision);
	else
		bas<<fixed(e*std::exp2(x*details::log2_minus_10),a.precision);
	auto& str(bas.str());
	if(str.find('.')!=std::string::npos)
	{
		for(;!str.empty()&&str.back()=='0';str.pop_back());
		if(!str.empty()&&str.back()=='.')
			str.pop_back();
		out<<str;
	}
	if(fix)
		return out;
	}
	if(x==0)
		return out;
	out.put('e');
	if(x<0)
	{
		out.put('-');
		return out<<static_cast<std::uint64_t>(-x);
	}
	return out<<static_cast<std::uint64_t>(x);
}

template<typename T>
requires std::numeric_limits<T>::is_iec559
standard_output_stream& operator<<(standard_output_stream& out,T v)
{
	return out<<floating_point_default(v,14);
}

}
#endif