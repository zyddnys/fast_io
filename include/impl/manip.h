#pragma once

#include<cmath>

namespace fast_io
{
namespace details
{
template<typename T>
struct char_view_t
{
	T& reference;
};

template<Integral T>
struct unsigned_view_t
{
	T& reference;
};

template<Integral T>
struct signed_view_t
{
	T& reference;
};

template<typename T>
struct setw_t
{
	std::size_t width;
	T& reference;
};

template<typename T,Integral char_type>
struct setw_fill_t
{
	std::size_t width;
	T& reference;
	char_type ch;
};
template<typename T>
struct fixed
{
public:
	T& reference;
	std::size_t precision;
};
template<typename T>
struct scientific
{
public:
	T& reference;
	std::size_t precision;
};
template<typename T>
struct floating_point_default
{
public:
	T& reference;
	std::size_t precision;
};
}
template<Integral T>
inline constexpr details::char_view_t<T> char_view(T& ch)
{
	return {ch};
}
template<Integral T>
inline constexpr details::char_view_t<T const> char_view(T const& ch)
{
	return {ch};
}

template<Integral T>
inline constexpr decltype(auto) unsigned_view(T& integral)
{
	return reinterpret_cast<std::make_unsigned_t<T>&>(integral);
}

template<Integral T>
inline constexpr decltype(auto) signed_view(T& integral)
{
	return reinterpret_cast<std::make_signed_t<T>&>(integral);
}

template<Integral T>
inline constexpr decltype(auto) unsigned_view(T const& integral)
{
	return reinterpret_cast<std::make_unsigned_t<T const>&>(integral);
}

inline constexpr decltype(auto) floating_view(Integral const& integral)
{
	return static_cast<long double>(integral);
}

inline constexpr decltype(auto) floating_view(Floating_point const& p)
{
	return p;
}

inline constexpr decltype(auto) unsigned_view(Floating_point const& f)
{
	return static_cast<std::uintmax_t>(f);
}

inline constexpr decltype(auto) signed_view(Floating_point const& f)
{
	return static_cast<std::intmax_t>(f);
}

template<Integral T>
inline constexpr decltype(auto) signed_view(T const& integral)
{
	return reinterpret_cast<std::make_signed_t<T const>&>(integral);
}

inline constexpr std::size_t unsigned_view(auto * const pointer)
{
	return reinterpret_cast<std::size_t>(pointer);
}

template<Floating_point T>
inline constexpr details::char_view_t<T const> char_view(T const& ch)
{
	return {ch};
}

template<Floating_point T>
inline constexpr details::char_view_t<T> char_view(T& ch)
{
	return {ch};
}

/*
template<Integral T>
inline constexpr details::unsigned_view_t<T const> unsigned_view(T const& integral)
{
	return {integral};
}

template<Integral T>
inline constexpr details::signed_view_t<T const> signed_view(T const& integral)
{
	return {integral};
}*/

//	template<template T>
//	requires requires std::is_floating_point_v<T>
template<typename T>
inline constexpr details::fixed<T const> fixed(T const &f,std::size_t precision)
{
	return {f,precision};
}
template<typename T>
inline constexpr details::scientific<T const> scientific(T const &f,std::size_t precision)
{
	return {f,precision};
}
template<typename T>
inline constexpr details::floating_point_default<T const> floating_point_default(T const &f,std::size_t precision)
{
	return {f,precision};
}

inline void scan(standard_input_stream& in,details::char_view_t<Integral> a)
{
	a.reference = in.get();
}

inline void print(standard_output_stream& out,details::char_view_t<Integral> a)
{
	out.put(a.reference);
}

inline void scan(standard_input_stream& in,details::char_view_t<Floating_point> a)
{
	a.reference = in.get();
}

inline void print(standard_output_stream& out,details::char_view_t<Floating_point> a)
{
	out.put(a.reference);
}

template<typename T>
inline details::setw_t<T const> setw(std::size_t width,T const&t)
{
	return {width,t};
}

template<typename T,Integral char_type>
inline constexpr details::setw_fill_t<T const,char_type> setw(std::size_t width,T const&t,char_type ch)
{
	return {width,t,ch};
}

template<standard_output_stream output>
inline void print(output& out,details::setw_fill_t<auto,Integral> a)
{
	basic_ostring<std::basic_string<typename output::char_type>> bas;
	print(bas,a.reference);
	for(std::size_t i(bas.str().size());i<a.width;++i)
		out.put(a.ch);
	print(out,bas.str());
}

template<standard_output_stream output>
inline void print(output& out,details::setw_t<auto> a)
{
	basic_ostring<std::basic_string<typename output::char_type>> bas;
	print(bas,a.reference);
	for(std::size_t i(bas.str().size());i<a.width;++i)
		out.put(' ');
	print(out,bas.str());
}

}

#include"floating.h"