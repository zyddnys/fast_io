#pragma once
#include"concept.h"
#include<bitset>
#include<stdexcept>
#include<array>
#include"stringbuf.h"

namespace fast_io
{

template<typename T,typename CharT>
requires input_stream<T>()||output_stream<T>()
class unicode_view
{
	T& ib;
public:
	using native_interface_t = T;
	using char_type = CharT;
	using native_char_type = typename native_interface_t::char_type;
public:
	constexpr unicode_view(T& ibv):ib(ibv){}
	constexpr auto& native_handle()
	{
		return ib;
	}
	constexpr auto eof() const requires standard_input_stream<T>()
	{
		return ib.eof();
	}
	constexpr char_type get() requires standard_input_stream<T>()
	{
		auto ch(ib.get());
		auto constexpr ch_bits(sizeof(native_char_type)*8);
		union
		{
			decltype(ib.get()) ch;
			std::bitset<ch_bits> bts;
		}u{ib.get()};
		if(!u.bts.test(ch_bits-1))
			return u.ch;
		auto constexpr ch_bits_m2(ch_bits-2);
		auto constexpr limitm1((static_cast<std::size_t>(1)<<ch_bits_m2)-1);
		if(!u.bts.test(ch_bits_m2))
			throw std::runtime_error("not a utf8 character");
		u.bts.reset(ch_bits-1);
		std::size_t pos(ch_bits_m2-1);
		for(;pos<ch_bits&&u.bts.test(pos);--pos)
			u.bts.reset(pos);
		std::size_t bytes(ch_bits_m2-pos);
		char_type converted_ch(u.fake_char);
		for(std::size_t i(0);i!=bytes;++i)
		{
			if(ib.eof())
				throw std::runtime_error("end of file before complete reading a utf8 character");
			auto t(ib.get());
			if((t>>ch_bits_m2)==2)
				converted_ch=(converted_ch<<ch_bits_m2)|(t&limitm1);
			else
				throw std::runtime_error("not a utf8 character");
		}
		return converted_ch;
	}
	template<typename Contiguous_iterator>
	constexpr Contiguous_iterator read(Contiguous_iterator b,Contiguous_iterator e) requires standard_input_stream<T>()
	{
		auto pb(static_cast<char_type*>(static_cast<void*>(std::addressof(*b))));
		auto pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));
		auto pi(pb);
		for(;pi!=pe;++pi)
			*pi=get();
		return b+(pi-pb)*sizeof(char_type)/sizeof(*b);
	}
	constexpr void put(char_type ch) requires standard_output_stream<T>()
	{
		auto constexpr bytes(sizeof(native_char_type)*8);
		auto constexpr lshift1(bytes-1);
		auto constexpr limit1(static_cast<std::size_t>(1)<<lshift1);
		if(ch<limit1)
		{
			ib.put(ch);
			return;
		}
		auto constexpr lshift2(bytes-2);
		auto constexpr limit2(static_cast<std::size_t>(1)<<lshift2);
		auto constexpr limitm2(limit2-1);

		std::array<native_char_type,sizeof(native_char_type)*8> a{};
		auto i(a.size()-1);
		for(;i<a.size()&&ch;--i)
		{
			a[i]=((ch&limitm2)|limit1);
			ch>>=lshift2;
		}
		if(a.size()<=++i)
			throw std::runtime_error("incorrect character");
		std::size_t pos(i-1);
		native_char_type v(0);
		v=~v;
		v>>=pos;
		v<<=pos;
		v&=~(1<<pos);
		a[i]|=v;
		a[i]&=~(1<<pos);
		ib.write(a.cbegin()+i,a.cend());
	}
	template<typename Contiguous_iterator>
	constexpr void write(Contiguous_iterator b,Contiguous_iterator e) requires standard_output_stream<T>()
	{
		write_precondition(b,e);
		auto pb(std::addressof(*b));
		for(auto pi(pb),pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));pi!=pe;++pi)
			put(*pi);
	}
	constexpr void flush() requires standard_output_stream<T>()
	{
		ib.flush();
	}
};

template<typename T>
inline constexpr void in_place_utf8_to_unicode(T& t,std::string_view view)
{
	basic_ibuf_string_view<std::string_view> ibsv(view);
	unicode_view<decltype(ibsv),typename T::value_type> uv(ibsv);
	getwhole(uv,t);
}

template<typename T=std::wstring>
inline constexpr auto utf8_to_unicode(std::string_view view)
{
	T t;
	in_place_utf8_to_unicode(t,view);
	return t;
}

namespace details
{
template<typename T>
inline void in_place_unicode_to_utf8(std::string& v,std::basic_string_view<T> view)
{
	basic_obuf_string<std::string> obsv(std::move(v));
	unicode_view<decltype(obsv),T> uv(obsv);
	uv<<view;
	v=std::move(obsv.str());
}

template<typename T>
inline std::string unicode_to_utf8(std::basic_string_view<T> view)
{
	basic_obuf_string<std::string> obsv;
	unicode_view<decltype(obsv),T> uv(obsv);
	uv<<view;
	return std::move(obsv.str());
}
}

inline void in_place_unicode_to_utf8(std::string& v,std::wstring_view view)
{
	details::in_place_unicode_to_utf8(v,view);
}

inline std::string unicode_to_utf8(std::wstring_view v)
{
	return details::unicode_to_utf8(v);
}

inline void in_place_unicode_to_utf8(std::string& v,std::u16string_view view)
{
	details::in_place_unicode_to_utf8(v,view);
}

inline std::string unicode_to_utf8(std::u16string_view v)
{
	return details::unicode_to_utf8(v);
}

inline void in_place_unicode_to_utf8(std::string& v,std::u32string_view view)
{
	details::in_place_unicode_to_utf8(v,view);
}

inline std::string unicode_to_utf8(std::u32string_view v)
{
	return details::unicode_to_utf8(v);
}

}