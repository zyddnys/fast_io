#pragma once

#include<string>
#include"concept.h"
#include<string_view>

namespace fast_io
{
template< typename T>
class basic_ibuf_string_view
{
	T s;
public:
	using value_type = T;
	using traits_type = typename value_type::traits_type;
private:
	using int_type = typename traits_type::int_type;
	using char_type = typename T::value_type;
public:
	template<typename ...Args>
	constexpr basic_ibuf_string_view(Args&& ...args):s(std::forward<Args>(args)...){}
	constexpr auto& str()
	{
		return s;
	}
	constexpr bool eof() const
	{
		return s.empty();
	}
	constexpr operator bool() const
	{
		return !s.empty();
	}
	template<typename contiguous_iterator>
	constexpr contiguous_iterator read(contiguous_iterator cbegin,contiguous_iterator cend)
	{
		std::size_t const cped(s.copy(std::addressof(*cbegin),cend-cbegin));
		s.remove_prefix(cped);
		return cbegin+cped;
	}
	constexpr int_type get()
	{
		if(eof())
			return traits_type::eof();
		int_type ch(s.front());
		s.remove_prefix(1);
		return traits_type::to_int_type(ch);
	}
};

template< typename T>
class basic_obuf_string
{
	T s;
public:
	using value_type = T;
	using traits_type = typename value_type::traits_type;
private:
	using int_type = typename traits_type::int_type;
	using char_type = typename T::value_type;
public:
	template<typename... Args>
	constexpr basic_obuf_string(Args&& ...args):s(std::forward<Args>(args)...){}
	constexpr auto& str()
	{
		return s;
	}
	template<typename contiguous_iterator>
	constexpr void write(contiguous_iterator cbegin,contiguous_iterator cend)
	{
		s.append(static_cast<char_type const*>(static_cast<void*>(std::addressof(*cbegin))),static_cast<char_type const*>(static_cast<void*>(std::addressof(*cend))));
	}
	constexpr void put(int_type ch)
	{
		s.push_back(traits_type::to_char_type(ch));
	}
	constexpr void flush(){}
	constexpr void clear(){s.clear();}
};

//potential constexpr in the future if std::string can be constexpr

template<typename T=std::string,typename... Args>
inline constexpr T concat(Args&& ...args)
{
	basic_obuf_string<T> t;
	print(t,std::forward<Args>(args)...);
	return std::move(t.str());
}
template<typename T=std::string,typename... Args>
inline constexpr T format(std::string_view format,Args&& ...args)
{
	basic_obuf_string<T> t;
	fprint(t,format,std::forward<Args>(args)...);
	return std::move(t.str());
}
template<typename T,typename... Args>
inline constexpr void inplace_to(T& t,Args&& ...args)
{
	basic_obuf_string<std::string> os;
	basic_ibuf_string_view<std::string_view> is(print(os,std::forward<Args>(args)...).str());
	is>>t;
}

template<typename... Args>
inline constexpr void in_place_to(std::string& t,Args&& ...args)
{
	basic_obuf_string<std::string> os(std::move(t));
	os.clear();
	print(os,std::forward<Args>(args)...);
	t=std::move(os.str());
} 



template<typename T,typename... Args>
inline constexpr auto to(Args&& ...args)
{
	T t;
	in_place_to(t,std::forward<Args>(args)...);
	return t;
}

}
