#pragma once

namespace fast_io
{

namespace details
{

template<stream T,typename CharT>
struct ucs_char_size_max_cal
{
	explicit ucs_char_size_max_cal() = default;
	static constexpr bool value = sizeof(typename T::char_type)<sizeof(CharT);
};

}

template<stream T,typename CharT>
requires details::ucs_char_size_max_cal<T,CharT>::value
class ucs_view
{
	T& ib;
public:
	using native_interface_t = T;
	using char_type = CharT;
	using native_char_type = typename native_interface_t::char_type;
private:
	using unsigned_char_type = std::make_unsigned_t<char_type>;
	using unsigned_native_char_type = std::make_unsigned_t<native_char_type>;
	constexpr char_type get_impl(unsigned_native_char_type ch)
		requires standard_input_stream<T>
	{
		auto constexpr ch_bits(sizeof(native_char_type)*8);
		union
		{
			unsigned_native_char_type ch;
			std::bitset<ch_bits> bts;
		}u{ch};
		if(!u.bts.test(ch_bits-1))
			return u.ch;
		auto constexpr ch_bits_m2(ch_bits-2);
		auto constexpr limitm1((static_cast<unsigned_native_char_type>(1)<<ch_bits_m2)-1);
		if(!u.bts.test(ch_bits_m2))
			throw std::runtime_error("not a utf8 character");
		u.bts.reset(ch_bits-1);
		std::size_t pos(ch_bits_m2-1);
		for(;pos<ch_bits&&u.bts.test(pos);--pos)
			u.bts.reset(pos);
		std::size_t bytes(ch_bits_m2-pos);
		unsigned_char_type converted_ch(u.ch);
		for(std::size_t i(0);i!=bytes;++i)
		{
			unsigned_native_char_type t(ib.get());
			if((t>>ch_bits_m2)==2)
				converted_ch=((converted_ch<<ch_bits_m2)|(t&limitm1))&((1<<((i+2)*6)) -1);
			else
				throw std::runtime_error("not a utf8 character");
		}
		return converted_ch;
	}
public:
	constexpr ucs_view(T& ibv):ib(ibv){}
	constexpr auto& native_handle()
	{
		return ib;
	}
	constexpr auto eof() const requires standard_input_stream<T>
	{
		return ib.eof();
	}
	constexpr char_type get() requires standard_input_stream<T>
	{
		return get_impl(ib.get());
	}
	constexpr std::pair<char_type,bool> try_get() requires standard_input_stream<T>
	{
		auto ch(ib.try_get());
		if(ch.second)
			return {0,true};
		return {get_impl(ch.first),false};
	}
	template<typename Contiguous_iterator>
	constexpr Contiguous_iterator read(Contiguous_iterator b,Contiguous_iterator e)
		requires standard_input_stream<T>
	{
		auto pb(static_cast<char_type*>(static_cast<void*>(std::addressof(*b))));
		auto pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));
		auto pi(pb);
		for(;pi!=pe;++pi)
			*pi=get();
		return b+(pi-pb)*sizeof(char_type)/sizeof(*b);
	}
	constexpr void put(char_type ch)
		requires standard_output_stream<T>
	{
		unsigned_native_char_type constexpr native_char_bits(8*sizeof(unsigned_native_char_type));
		unsigned_native_char_type constexpr fair(1<<(native_char_bits-1));
		unsigned_native_char_type constexpr utf8_limit(1<<(native_char_bits-2));
		if(ch<fair)
		{
			ib.put(ch);
			return;
		}
		std::array<unsigned_native_char_type,sizeof(char_type)/sizeof(unsigned_native_char_type)+1> v{};
		auto ed(v.data()+v.size());
		do
		{
			*--ed = (ch%utf8_limit)|fair;
		}
		while(ch/=utf8_limit);
		std::size_t v_elements(v.data()+v.size()-ed);
		if((1<<(native_char_bits-1-v_elements))<=(*ed&~fair))
		{
			--ed;
			++v_elements;
		}
		unsigned_native_char_type constexpr max_native_char_type(-1);
		*ed |= max_native_char_type>>(native_char_bits-v_elements-1)<<(native_char_bits-v_elements);
		ib.write(ed,v.data()+v.size());
	}
	template<typename Contiguous_iterator>
	constexpr void write(Contiguous_iterator b,Contiguous_iterator e)
		requires standard_output_stream<T>
	{
		write_precondition<char_type>(b,e);
		auto pb(static_cast<char_type const*>(static_cast<void const*>(std::addressof(*b))));
		for(auto pi(pb),pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));pi!=pe;++pi)
			put(*pi);
	}
	constexpr void flush()
		requires standard_output_stream<T>
	{
		ib.flush();
	}
};

template<typename T>
inline constexpr void in_place_utf8_to_ucs(T& t,std::string_view view)
{
	basic_istring_view<std::string_view> ibsv(view);
	ucs_view<decltype(ibsv),typename T::value_type> uv(ibsv);
	getwhole(uv,t);
}

template<typename T=std::wstring>
inline constexpr auto utf8_to_ucs(std::string_view view)
{
	T t;
	in_place_utf8_to_ucs(t,view);
	return t;
}

namespace details
{
template<typename T>
inline void in_place_ucs_to_utf8(std::string& v,std::basic_string_view<T> view)
{
	basic_ostring<std::string> obsv(std::move(v));
	ucs_view<decltype(obsv),T> uv(obsv);
	uv.write(view.cbegin(),view.cend());
	v=std::move(obsv.str());
}

template<typename T>
inline std::string ucs_to_utf8(std::basic_string_view<T> view)
{
	basic_ostring<std::string> obsv;
	ucs_view<decltype(obsv),T> uv(obsv);
	uv.write(view.cbegin(),view.cend());
	return std::move(obsv.str());
}
}

inline void in_place_ucs_to_utf8(std::string& v,std::wstring_view view)
{
	details::in_place_ucs_to_utf8(v,view);
}

inline std::string ucs_to_utf8(std::wstring_view v)
{
	return details::ucs_to_utf8(v);
}

inline void in_place_ucs_to_utf8(std::string& v,std::u16string_view view)
{
	details::in_place_ucs_to_utf8(v,view);
}

inline std::string ucs_to_utf8(std::u16string_view v)
{
	return details::ucs_to_utf8(v);
}

inline void in_place_ucs_to_utf8(std::string& v,std::u32string_view view)
{
	details::in_place_ucs_to_utf8(v,view);
}

inline std::string ucs_to_utf8(std::u32string_view v)
{
	return details::ucs_to_utf8(v);
}

}