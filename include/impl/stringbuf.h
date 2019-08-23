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
		std::size_t const cped(s.copy(begin,end-begin));
		s.remove_prefix(cped);
		return begin+cped;
	}
	constexpr int_type get()
	{
		if(eof())
			return traits_type::eof();
		int_type ch(s.front());
		s.remove_prefix(1);
		return ch;
	}
};

template< typename T>
class basic_obuf_string
{
	T s;
public:
	using value_type = T;
	using traits_type = typename value_type::traits_type;
	using int_type = typename traits_type::int_type;
	using char_type = typename T::value_type;
	template<typename... Args>
	basic_obuf_string(Args&& ...args):s(std::forward<Args>(args)...){}
	auto& str()
	{
		return s;
	}
	template<typename contiguous_iterator>
	void write(contiguous_iterator cbegin,contiguous_iterator cend)
	{
		s.append(static_cast<char_type const*>(static_cast<void*>(std::addressof(*cbegin))),static_cast<char_type const*>(static_cast<void*>(std::addressof(*cend))));
	}
	template<typename V>
	void put(V ch)
	{
		s.push_back(ch);
	}
	void flush(){}
	void clear(){s.clear();}
};

//potential constexpr in the future if std::string can be constexpr

template<typename T=std::string,typename... Args>
inline T concat(Args&& ...args)
{
	basic_obuf_string<T> t;
	print(t,std::forward<Args>(args)...);
	return std::move(t.str());
}
template<typename T=std::string,typename... Args>
inline T format(std::string_view format,Args&& ...args)
{
	basic_obuf_string<T> t;
	fprint(t,format,std::forward<Args>(args)...);
	return std::move(t.str());
}
template<typename T,typename... Args>
inline void inplace_to(T& t,Args&& ...args)
{
	basic_obuf_string<std::string> os;
	basic_ibuf_string_view<std::string_view> is(print(os,std::forward<Args>(args)...).str());
	is>>t;
}

template<typename... Args>
inline void in_place_to(std::string& t,Args&& ...args)
{
	basic_obuf_string<std::string&> os(t);
	os.clear();
	print(os,std::forward<Args>(args)...);
}

template<typename T,typename... Args>
inline auto to(Args&& ...args)
{
	T t;
	in_place_to(t,std::forward<Args>(args)...);
	return t;
}

}
