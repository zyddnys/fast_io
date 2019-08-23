#pragma once
#include"concept.h"
#include<bitset>
#include<stdexcept>
#include<array>
namespace fast_io
{

template<typename in,typename CharT,typename Traits = std::char_traits<CharT>>
class unicode_view
{
	in& ib;
public:
	using native_handle_type = in&;
	using char_type = CharT;
	using char_traits = Traits;
	using int_type = typename Traits::int_type;
	constexpr unicode_view(in& ibv):ib(ibv){}
	constexpr auto& native_handle()
	{
		return ib;
	}
	constexpr auto eof() const requires(standard_input_stream<in>())
	{
		return ib.eof();
	}
	constexpr operator bool() const requires(standard_input_stream<in>())
	{
		return !ib;
	}
	constexpr int_type get() requires(standard_input_stream<in>())
	{
		auto ch(ib.get());
		if(ch==in::char_traits::eof())
			return char_traits::eof();
		auto constexpr ch_bits(sizeof(ch)*8);
		using fake_char_t = std::make_unsigned_t<typename in::char_type>;
		union
		{
			fake_char_t fake_char;
			std::bitset<ch_bits> bts;
		}u{static_cast<fake_char_t>(ch)};
		if(!u.bts.test(ch_bits-1))
			return u.fake_char;
		auto constexpr ch_bits_m2(ch_bits-2);
		if(!u.bts.test(ch_bits_m2))
			throw std::runtime_error("not a utf8 character");
		std::size_t pos(ch_bits_m2-1);
		for(;pos<ch_bits&&u.bts.test(pos);--pos)
			u.bts.reset(pos);
		std::size_t bytes(ch_bits_m2-pos);
		char_type converted_ch(u.fake_char);
		for(std::size_t i(0);i!=bytes;++i)
		{
			auto c(ib.get());
			if(c==in::char_traits::eof())
				throw std::runtime_error("end of file before complete reading a utf8 character");
			fake_char_t t{static_cast<fake_char_t>(c)};
			if((t>>ch_bits_m2)==2)
				converted_ch=(converted_ch<<ch_bits_m2)|((t<<2)>>2);
			else
				throw std::runtime_error("not a utf8 character");
		}
		return converted_ch;
	}
	template<typename Contiguous_iterator>
	constexpr Contiguous_iterator read(Contiguous_iterator b,Contiguous_iterator e) requires(standard_input_stream<in>())
	{
		for(auto i(b);i!=e;++i)
		{
			union
			{
				std::array<decltype(*b),sizeof(char_type)> a;
				std::array<char_type,sizeof(decltype(*b))> b;
			}v;
			auto j(v.b.begin()),je(std::min(j+(e-i)*sizeof(decltype(*b))/sizeof(char),b.end()));
			for(;j!=je;++j)
			{
				auto ch(get());
				if(ch==char_traits::eof())
					break;
				*j=ch;
			}
			auto pj(v.a.begin()+(j-v.b.begin())*sizeof(char)/sizeof(decltype(*b)));
			for(auto j(v.a.begin());j!=pj;++j)
			{
				*i=std::move(*j);
				++i;
			}
			if(j!=je)
				return i;
		}
		return ib.read(b,e);
	}
};

/*
template<standard_output_stream out,typename CharT,typename Traits = std::char_traits<CharT>>
class ounicode_view
{
	
};*/

}