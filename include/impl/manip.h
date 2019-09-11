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

template<std::integral T>
struct unsigned_view_t
{
	T& reference;
};

template<std::integral T>
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

template<typename T,std::integral char_type>
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
template<std::integral T>
inline constexpr details::char_view_t<T> char_view(T& ch)
{
	return {ch};
}
template<std::integral T>
inline constexpr details::char_view_t<T const> char_view(T const& ch)
{
	return {ch};
}

template<std::integral T>
inline constexpr decltype(auto) unsigned_view(T& value)
{
	return reinterpret_cast<std::make_unsigned_t<T>&>(value);
}

template<std::integral T>
inline constexpr decltype(auto) signed_view(T& value)
{
	return reinterpret_cast<std::make_signed_t<T>&>(value);
}

template<std::integral T>
inline constexpr decltype(auto) unsigned_view(T const& value)
{
	return reinterpret_cast<std::make_unsigned_t<T const>&>(value);
}

template<std::integral T>
inline constexpr decltype(auto) floating_view(T const& value)
{
	return static_cast<long double>(value);
}

template<std::floating_point F>
inline constexpr decltype(auto) floating_view(F const& p)
{
	return p;
}

template<std::floating_point F>
inline constexpr decltype(auto) unsigned_view(F const& f)
{
	return static_cast<std::uintmax_t>(f);
}

template<std::floating_point F>
inline constexpr decltype(auto) signed_view(F const& f)
{
	return static_cast<std::intmax_t>(f);
}

template<std::integral T>
inline constexpr decltype(auto) signed_view(T const& value)
{
	return reinterpret_cast<std::make_signed_t<T const>&>(value);
}

template<typename T>
inline constexpr std::size_t unsigned_view(T * const pointer)
{
	return reinterpret_cast<std::size_t>(pointer);
}

template<std::floating_point T>
inline constexpr details::char_view_t<T const> char_view(T const& ch)
{
	return {ch};
}

template<std::floating_point T>
inline constexpr details::char_view_t<T> char_view(T& ch)
{
	return {ch};
}

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

template<standard_input_stream input,std::integral T>
inline void scan(input& in,details::char_view_t<T> a)
{
	a.reference = in.get();
}

template<standard_output_stream output,std::integral T>
inline void print(output& out,details::char_view_t<T> a)
{
	out.put(static_cast<typename output::char_type>(a.reference));
}
template<standard_input_stream input,std::floating_point T>
inline void scan(input& in,details::char_view_t<T> a)
{
	a.reference = in.get();
}

template<standard_output_stream output,std::floating_point T>
inline void print(output& out,details::char_view_t<T> a)
{
	out.put(static_cast<typename output::char_type>(a.reference));
}

template<typename T>
inline details::setw_t<T const> setw(std::size_t width,T const&t)
{
	return {width,t};
}

template<typename T,std::integral char_type>
inline constexpr details::setw_fill_t<T const,char_type> setw(std::size_t width,T const&t,char_type ch)
{
	return {width,t,ch};
}

template<standard_output_stream output,typename T,std::integral U>
inline void print(output& out,details::setw_fill_t<T,U> a)
{
	basic_ostring<std::basic_string<typename output::char_type>> bas;
	print(bas,a.reference);
	for(std::size_t i(bas.str().size());i<a.width;++i)
		out.put(a.ch);
	print(out,bas.str());
}

template<standard_output_stream output,typename T>
inline void print(output& out,details::setw_t<T> a)
{
	basic_ostring<std::basic_string<typename output::char_type>> bas;
	print(bas,a.reference);
	for(std::size_t i(bas.str().size());i<a.width;++i)
		out.put(' ');
	print(out,bas.str());
}

}

#include"floating.h"