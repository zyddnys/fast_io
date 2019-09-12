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

template<std::uint8_t base,bool uppercase,standard_output_stream output,std::unsigned_integral U>
inline void output_base_number(output& out,U a)
{
//number: 0:48 9:57
//upper: 65 :A 70: F
//lower: 97 :a 102 :f
	if(a)
	{
		std::array<typename output::char_type,sizeof(a)*8> v;
		auto ed(v.data()+v.size());
		do
		{
			if constexpr(10 < base)
			{
				--ed;
				auto temp(a%base);
				if(temp<10)
					*ed = static_cast<typename output::char_type>(temp+48);
				else
				{
					if constexpr (uppercase)
						*ed = static_cast<typename output::char_type>(temp+55);	
					else
						*ed = static_cast<typename output::char_type>(temp+87);
				}
			}
			else
				*--ed = static_cast<typename output::char_type>(a%base+48);
		}
		while(a/=base);
		out.write(ed,v.data()+v.size());
	}
	else
		out.put(48);
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

template<std::uint8_t base,standard_input_stream input,std::unsigned_integral U>
inline constexpr void input_base_number(input& in,U& a)
{
//	auto constexpr baseed(48+base);
	while(true)
	{
		std::make_unsigned_t<decltype(in.get())> ch(in.get());
		if(ch-48<base)
		{
			a=ch-48;
			break;
		}
		else if constexpr (10 < base)
		{
			auto constexpr basem10(base-10);
			if(ch-65<basem10)
			{
				a=ch-55;
				break;
			}
			else	if(ch-97<basem10)
			{
				a=ch-87;
				break;
			}
		}
	}
	while(true)
	{
		auto try_ch(in.try_get());
		std::make_unsigned_t<decltype(in.get())> ch(try_ch.first);
		ch-=48;
		if(ch<10)
			a=a*base+ch;
		else if constexpr (10 < base)
		{
			auto constexpr basem10(base-10);
			if(ch-17<basem10)
				a=a*base+(ch-7);
			else if(ch-49<basem10)
				a=a*base+(ch-39);
			else
				return;
		}
		else
			return;
	}
}
template<std::uint8_t base,standard_input_stream input, std::signed_integral T>
inline constexpr void input_base_number(input& in,T& a)
{
	auto constexpr baseed(48+base);
	bool rev(false);
	while(true)
	{
		auto ch(in.get());
		if(48<=ch&&ch<baseed)
		{
			a=ch-48;
			break;
		}
		else if(ch==45)
		{
			a=0;
			rev=true;
			break;
		}
		else if constexpr (10 < base)
		{
			if(65<=ch&&ch<65+base)
			{
				a=ch-55;
				break;
			}
			else if(97<=ch&&ch<97+base)
			{
				a=ch-87;
				break;
			}
		}
	}
	while(true)
	{
		auto try_ch(in.try_get());
		if(48<=try_ch.first&&try_ch.first<baseed)
			a=a*base+try_ch.first-48;
		else if constexpr (10 < base)
		{
			if(65<=try_ch.first&&try_ch.first<65+base)
				a=a*base+try_ch.first-55;
			else if(97<=try_ch.first&&try_ch.first<97+base)
				a=a*base+try_ch.first-87;
			else
				break;
		}
		else
			break;
	}
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