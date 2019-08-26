#pragma once

namespace fast_io
{

namespace details
{

template<std::size_t bs,bool uppercase,typename T>
requires 2<=bs&&bs<=36&&((bs<10&&!uppercase)||10<=bs)
struct base_t
{
	T& reference;
};

}

template<std::size_t b,bool uppercase=false,typename T>
inline constexpr details::base_t<b,uppercase,T> base(T& t) {return {t};}
template<std::size_t b,bool uppercase=false,typename T>
inline constexpr details::base_t<b,uppercase,T const> base(T const& t) {return {t};}

template<typename T> inline constexpr details::base_t<16,false,T> hex(T& t) {return {t};}
template<typename T> inline constexpr details::base_t<16,false,T const> hex(T const& t){return {t};}

template<typename T> inline constexpr details::base_t<16,true,T> hexupper(T& t){return {t};}
template<typename T> inline constexpr details::base_t<16,true,T const> hexupper(T const& t) {return {t};}

template<typename T> inline constexpr details::base_t<8,false,T> oct(T& t) {return {t};}
template<typename T> inline constexpr details::base_t<8,false,T const> oct(T const& t){return {t};}

template<typename T> inline constexpr details::base_t<10,false,T> dec(T& t) {return {t};}
template<typename T> inline constexpr details::base_t<10,false,T const> dec(T const& t){return {t};}

template<typename T> inline constexpr details::base_t<2,false,T> bin(T& t){return {t};}
template<typename T> inline constexpr details::base_t<2,false,T const> bin(T const& t) {return {t};}


template<std::size_t base,bool uppercase>
inline constexpr standard_output_stream& operator<<(standard_output_stream& out,details::base_t<base,uppercase,Unsigned_integer> v)
{
	return details::output_unsigned_base_number<base,uppercase>(out,v.reference);
}

template<std::size_t base,bool uppercase>
inline constexpr standard_output_stream& operator<<(standard_output_stream& out,details::base_t<base,uppercase,Signed_integer> v)
{
	return details::output_signed_base_number<base,uppercase>(out,v.reference);
}

}