#pragma once


namespace fast_io
{
template< typename T>
class basic_istring_view
{
	T s;
public:
	using char_type = typename T::value_type;
	template<typename ...Args>
	requires std::constructible_from<T,Args...>
	constexpr basic_istring_view(Args&& ...args):s(std::forward<Args>(args)...){}
	constexpr auto& str()
	{
		return s;
	}
	constexpr auto empty() const {return s.empty();}
};

template<typename T>
[[nodiscard]] inline constexpr auto ireserve(basic_istring_view<T>& isv,std::size_t size)
{
	isv.str().remove_prefix(size);
	return isv.str().begin();
}

template<typename T,std::contiguous_iterator Iter>
inline constexpr Iter reads(basic_istring_view<T>& istrvw,Iter begin,Iter end)
{
	auto pb(static_cast<typename T::value_type*>(static_cast<void*>(std::to_address(begin))));
	auto pe(static_cast<typename T::value_type*>(static_cast<void*>(std::to_address(end))));
	std::size_t const cped(istrvw.str().copy(pb,pe-pb));
	istrvw.str().remove_prefix(cped);
	return begin+cped*sizeof(*begin)/sizeof(typename T::value_type);
}

template<typename T>
inline constexpr typename T::value_type get(basic_istring_view<T>& istrvw)
{
	if(istrvw.empty())
		throw eof();
	auto ch(istrvw.str().front());
	istrvw.str().remove_prefix(1);
	return ch;
}
template<typename T>
inline constexpr std::pair<typename T::value_type,bool> try_get(basic_istring_view<T>& istrvw)
{
	if(istrvw.empty())
		return {0,true};
	auto ch(istrvw.str().front());
	istrvw.str().remove_prefix(1);
	return {ch,false};
}

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
	constexpr void clear(){s.clear();}
	constexpr auto empty() const {return s.empty();}
};

template<typename T>
[[nodiscard]] inline constexpr auto oreserve(basic_ostring<T>& ob,std::size_t size)
{
	ob.str().append(size,0);
	return ob.str().end();
}

template<typename T,std::contiguous_iterator Iter>
inline constexpr void writes(basic_ostring<T>& ostr,Iter cbegin,Iter cend)
{
	using char_type = typename T::value_type;
	writes_precondition<char_type>(cbegin,cend);
	ostr.str().append(static_cast<char_type const*>(static_cast<void const*>(std::to_address(cbegin))),static_cast<char_type const*>(static_cast<void const*>(std::to_address(cend))));
}
template<typename T>
inline constexpr void put(basic_ostring<T>& ostr,typename T::value_type ch)
{
	ostr.str().push_back(ch);
}

template<typename T>
inline constexpr void flush(basic_ostring<T>&){}

template<typename T>
inline constexpr void fill_nc(basic_ostring<T>& os,std::size_t count,typename T::value_type const& ch)
{
	os.str().append(count,ch);
}

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
