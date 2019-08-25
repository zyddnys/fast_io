#pragma once
#include"concept.h"
#include<bitset>
#include<stdexcept>
#include<array>
namespace fast_io
{

template<typename T,typename CharT,typename Traits = std::char_traits<CharT>>
requires input_stream<T>()||output_stream<T>()
class unicode_view
{
	T& ib;
public:
	using native_interface_t = T;
	using traits_type = typename native_interface_t::traits_type;
private:
	using char_type = typename traits_type::char_type;
	using int_type = typename traits_type::int_type;
public:
	constexpr unicode_view(T& ibv):ib(ibv){}
	constexpr auto& native_handle()
	{
		return ib;
	}
	constexpr auto eof() const requires(standard_input_stream<T>())
	{
		return ib.eof();
	}
	constexpr int_type get() requires(standard_input_stream<T>())
	{
		auto ch(ib.get());
		if(ch==traits_type::eof())
			return traits_type::eof();
		auto constexpr ch_bits(sizeof(ch)*8);
		using fake_char_t = std::make_unsigned_t<char_type>;
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
			if(c==traits_type::eof())
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
	constexpr Contiguous_iterator read(Contiguous_iterator b,Contiguous_iterator e) requires input_stream<T>()
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
				if(ch==traits_type::eof())
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