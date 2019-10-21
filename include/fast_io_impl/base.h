#pragma once

namespace fast_io
{

namespace details
{

template<std::size_t bs,bool uppercase=false>
struct base_number_upper_constraints
{
	explicit base_number_upper_constraints() = default;
	static constexpr bool value = 2<=bs&&bs<=36&&((bs<=10&&!uppercase)||10<bs);
};

template<std::uint8_t base,bool uppercase,output_stream output,std::unsigned_integral U>
inline void output_base_number(output& out,U a)
{
//number: 0:48 9:57
//upper: 65 :A 70: F
//lower: 97 :a 102 :f

	std::array<typename output::char_type,sizeof(a)*8> v;
	auto iter(v.data()+v.size()-1);
	for(;base<=a;--iter)
	{
		auto const quo(a/base);
		auto const rem(a%base);
		if constexpr(10 < base)
		{
			if(rem<10)
				*iter = static_cast<typename output::char_type>(rem+48);
			else
			{
				if constexpr (uppercase)
					*iter = static_cast<typename output::char_type>(rem+55);	
				else
					*iter = static_cast<typename output::char_type>(rem+87);
			}
		}
		else
			*iter = static_cast<typename output::char_type>(rem+48);
		a=quo;
	}
	if constexpr(10 < base)
	{
		if(a<10)
			*iter = static_cast<typename output::char_type>(a+48);
		else
		{
			if constexpr (uppercase)
				*iter = static_cast<typename output::char_type>(a+55);	
			else
				*iter = static_cast<typename output::char_type>(a+87);
		}
	}
	else
		*iter=static_cast<typename output::char_type>(a+48);
	out.writes(iter,v.data()+v.size());
}

template<std::uint8_t base,bool uppercase,standard_output_stream output,std::signed_integral T>
inline void output_base_number(output& out,T a)
{
	if(a<0)
	{
		out.put('-');
		a=-a;
	}
	output_base_number<base,uppercase>(out,static_cast<std::make_unsigned_t<T>>(a));
}

template<std::uint8_t base,standard_input_stream input,std::integral U>
inline constexpr void input_base_number_phase2(input& in,U& a)
{
	using unsigned_char_type = std::make_unsigned_t<decltype(in.get())>;
	unsigned_char_type constexpr baseed(std::min(static_cast<unsigned_char_type>(base),static_cast<unsigned_char_type>(10)));
	while(true)
	{
		unsigned_char_type ch(in.try_get().first);
		if((ch-=48)<baseed)
			a=a*base+ch;
		else if constexpr (10 < base)
		{
			unsigned_char_type constexpr bm10(base-10);
			if((ch-=17)<bm10||(ch-=32)<bm10)
				a=a*base+(ch+10);
			else
				return;
		}
		else
			return;
	}
}

template<std::uint8_t base,standard_input_stream input,std::unsigned_integral U>
inline constexpr void input_base_number(input& in,U& a)
{
	using unsigned_char_type = std::make_unsigned_t<decltype(in.get())>;
	unsigned_char_type constexpr baseed(std::min(static_cast<unsigned_char_type>(base),static_cast<unsigned_char_type>(10)));
	while(true)
	{
		unsigned_char_type ch(in.get());
		if((ch-=48)<baseed)
		{
			a=ch;
			break;
		}
		else if constexpr (10 < base)
		{
			unsigned_char_type constexpr bm10(base-10);
			if((ch-=17)<bm10||(ch-=32)<bm10)
			{
				a=ch+10;
				break;
			}
		}
	}
	input_base_number_phase2<base>(in,a);
}
template<std::uint8_t base,standard_input_stream input, std::signed_integral T>
inline constexpr void input_base_number(input& in,T& a)
{
	using unsigned_char_type = std::make_unsigned_t<decltype(in.get())>;
	unsigned_char_type constexpr baseed(std::min(static_cast<unsigned_char_type>(base),static_cast<unsigned_char_type>(10)));
	bool rev(false);
	while(true)
	{
		unsigned_char_type ch(in.get());
		if(ch==45)
		{
			a=0;
			rev=true;
			break;
		}
		else if((ch-=48)<baseed)
		{
			a=ch;
			break;
		}
		else if constexpr (10 < base)
		{
			unsigned_char_type constexpr bm10(base-10);
			if((ch-=17)<bm10||(ch-=32)<bm10)
			{
				a=ch+10;
				break;
			}
		}
	}
	input_base_number_phase2<base>(in,a);
	if(rev)
		a=-a;
}

template<std::size_t bs,bool uppercase,typename T>
requires base_number_upper_constraints<bs,uppercase>::value
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

template<std::size_t base,bool uppercase,standard_output_stream output,std::integral T>
inline constexpr void print(output& out,details::base_t<base,uppercase,T> v)
{
	details::output_base_number<base,uppercase>(out,v.reference);
}

template<std::size_t base,bool uppercase,standard_input_stream input,std::integral T>
inline constexpr void scan(input& in,details::base_t<base,uppercase,T> v)
{
	details::input_base_number<base>(in,v.reference);
}


template<standard_input_stream input,std::integral T>
inline void scan(input& in,T& a)
{
	details::input_base_number<10>(in,a);
}

template<standard_output_stream output,std::integral T>
inline constexpr void print(output& out,T const& a)
{
	details::output_base_number<10,false>(out,a);
}

}