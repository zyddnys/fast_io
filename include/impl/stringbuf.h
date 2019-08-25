#pragma once

#include<string>
#include"concept.h"
#include<string_view>
#include"write_precondition.h"

namespace fast_io
{
template< typename T>
class basic_ibuf_string_view
{
	T s;
public:
	using value_type = T;
	using char_type = std::make_unsigned_t<typename T::value_type>;
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
	template<typename contiguous_iterator>
	constexpr contiguous_iterator read(contiguous_iterator begin,contiguous_iterator end)
	{
		auto pb(static_cast<char_type*>(static_cast<void*>(std::addressof(*begin))));
		auto pe(static_cast<char_type*>(static_cast<void*>(std::addressof(*end))));
		std::size_t const cped(s.copy(pb,pe-pb));
		s.remove_prefix(cped);
		return begin+cped*sizeof(*begin)/sizeof(char_type);
	}
	constexpr char_type get()
	{
		if(s.empty())
			throw std::runtime_error("Try to read data from EOF ibuf string view");
		auto ch(s.front());
		s.remove_prefix(1);
		return ch;
	}
	constexpr std::pair<char_type,bool> try_get()
	{
		if(s.empty())
			return {0,true};
		auto ch(s.front());
		s.remove_prefix(1);
		return {ch,false};
	}
};

template< typename T>
class basic_obuf_string
{
	T s;
public:
	using value_type = T;
	using char_type = typename T::value_type;
	template<typename... Args>
	constexpr basic_obuf_string(Args&& ...args):s(std::forward<Args>(args)...){}
	constexpr auto& str()
	{
		return s;
	}
	template<typename contiguous_iterator>
	constexpr void write(contiguous_iterator cbegin,contiguous_iterator cend)
	{
		write_precondition<char_type>(cbegin,cend);
		s.append(static_cast<char_type const*>(static_cast<void const*>(std::addressof(*cbegin))),static_cast<char_type const*>(static_cast<void const*>(std::addressof(*cend))));
	}
	constexpr void put(char_type ch)
	{
		s.push_back(ch);
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
inline constexpr void in_place_to(T& t,Args&& ...args)
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
