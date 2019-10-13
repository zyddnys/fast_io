#pragma once


namespace fast_io
{
template< typename T>
class basic_istring_view
{
	T s;
public:
	using value_type = T;
	using char_type = std::make_unsigned_t<typename T::value_type>;
	template<typename ...Args>
	requires std::constructible_from<T,Args...>
	constexpr basic_istring_view(Args&& ...args):s(std::forward<Args>(args)...){}
	constexpr auto& str()
	{
		return s;
	}
	template<std::contiguous_iterator Iter>
	constexpr Iter reads(Iter begin,Iter end)
	{
		auto pb(static_cast<char_type*>(static_cast<void*>(std::to_address(begin))));
		auto pe(static_cast<char_type*>(static_cast<void*>(std::to_address(end))));
		std::size_t const cped(s.copy(pb,pe-pb));
		s.remove_prefix(cped);
		return begin+cped*sizeof(*begin)/sizeof(char_type);
	}
	constexpr char_type get()
	{
		if(s.empty())
			throw eof();
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
class basic_ostring
{
	T s;
public:
	using value_type = T;
	using char_type = typename T::value_type;
	template<typename ...Args>
	requires std::constructible_from<T,Args...>
	constexpr basic_ostring(Args&& ...args):s(std::forward<Args>(args)...){}
	constexpr auto& str()
	{
		return s;
	}
	template<std::contiguous_iterator Iter>
	constexpr void writes(Iter cbegin,Iter cend)
	{
		writes_precondition<char_type>(cbegin,cend);
		s.append(static_cast<char_type const*>(static_cast<void const*>(std::to_address(cbegin))),static_cast<char_type const*>(static_cast<void const*>(std::to_address(cend))));
	}
	constexpr void put(char_type ch)
	{
		s.push_back(ch);
	}
	constexpr void flush() const {}
	constexpr void clear(){s.clear();}
	constexpr auto empty() const {return s.empty();}
};

//potential constexpr in the future if std::string can be constexpr

template<typename T=std::string,typename... Args>
inline constexpr T concat(Args&& ...args)
{
	basic_ostring<T> t;
	print(t,std::forward<Args>(args)...);
	return std::move(t.str());
}

template<typename T=std::string,typename... Args>
inline constexpr T concatln(Args&& ...args)
{
	basic_ostring<T> t;
	println(t,std::forward<Args>(args)...);
	return std::move(t.str());
}

template<typename T=std::string,typename... Args>
inline constexpr T format(std::string_view format,Args&& ...args)
{
	basic_ostring<T> t;
	fprint(t,format,std::forward<Args>(args)...);
	return std::move(t.str());
}

template<typename T,typename... Args>
inline void in_place_to(T& t,Args&& ...args)
{
	basic_ostring<std::string> os;
	print(os,std::forward<Args>(args)...);
	basic_istring_view<std::string_view> is(os.str());
	scan(is,t);
}

template<typename... Args>
inline void in_place_to(std::string& t,Args&& ...args)
{
	basic_ostring<std::string> os(std::move(t));
	os.clear();
	print(os,std::forward<Args>(args)...);
	t=std::move(os.str());
}

template<typename T,typename... Args>
inline auto to(Args&& ...args)
{
	T t;
	in_place_to(t,std::forward<Args>(args)...);
	return t;
}

}
