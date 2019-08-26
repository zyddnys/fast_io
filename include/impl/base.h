#pragma once

namespace fast_io
{

namespace details
{
	
template<char base,bool uppercase,standard_output_stream output>
inline output& output_base_number(output& out,Unsigned_integer a)
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
					*ed = temp+48;
				else
				{
					if constexpr (uppercase)
						*ed = temp+55;	
					else
						*ed = temp+87;
				}
			}
			else
				*--ed = a%base+48;
		}
		while(a/=base);
		out.write(ed,v.data()+v.size());
	}
	else
		out.put(48);
	return out;
}

template<char base,bool uppercase,Signed_integer T>
inline standard_output_stream& output_base_number(standard_output_stream& out,T a)
{
	if(a<0)
	{
		out.put('-');
		a=-a;
	}
	return output_base_number<base,uppercase>(out,static_cast<std::make_unsigned_t<T>>(a));
}

template<char base>
inline constexpr standard_input_stream& input_base_number(standard_input_stream& in,Unsigned_integer& a)
{
	auto constexpr baseed(48+base);
	while(true)
	{
		auto ch(in.get());
		if(48<=ch&&ch<=baseed)
		{
			a=ch-48;
			break;
		}
		else if constexpr (10 < base)
		{
			if(65<=ch&&ch<=65+base)
			{
				a=ch-55;
				break;
			}
			else if(97<=ch&&ch<=97+base)
			{
				a=ch-87;
				break;
			}
		}
	}
	while(true)
	{
		auto try_ch(in.try_get());
		if(48<=try_ch.first&&try_ch.first<=baseed)
			a=a*base+try_ch.first-48;
		else if constexpr (10 < base)
		{
			if(65<=try_ch.first&&try_ch.first<=65+base)
				a=a*base+try_ch.first-55;
			else if(97<=try_ch.first&&try_ch.first<=97+base)
				a=a*base+try_ch.first-87;
			else
				break;
		}
		else
			break;
	}
	return in;
}
template<char base>
inline constexpr standard_input_stream& input_base_number(standard_input_stream& in,Signed_integer& a)
{
	auto constexpr baseed(48+base);
	bool rev(false);
	while(true)
	{
		auto ch(in.get());
		if(48<=ch&&ch<=baseed)
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
			if(65<=ch&&ch<=65+base)
			{
				a=ch-55;
				break;
			}
			else if(97<=ch&&ch<=97+base)
			{
				a=ch-87;
				break;
			}
		}
	}
	while(true)
	{
		auto try_ch(in.try_get());
		if(48<=try_ch.first&&try_ch.first<=baseed)
			a=a*base+try_ch.first-48;
		else if constexpr (10 < base)
		{
			if(65<=try_ch.first&&try_ch.first<=65+base)
				a=a*base+try_ch.first-55;
			else if(97<=try_ch.first&&try_ch.first<=97+base)
				a=a*base+try_ch.first-87;
			else
				break;
		}
		else
			break;
	}
	if(rev)
		a=-a;
	return in;
}

template<std::size_t bs,bool uppercase,typename T>
requires 2<=bs&&bs<=36&&((bs<=10&&!uppercase)||10<bs)
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

template<std::size_t base,bool uppercase>
inline constexpr standard_output_stream& operator<<(standard_output_stream& out,details::base_t<base,uppercase,Integral> v)
{
	return details::output_base_number<base,uppercase>(out,v.reference);
}

template<std::size_t base,bool uppercase>
inline constexpr standard_input_stream& operator>>(standard_input_stream& in,details::base_t<base,uppercase,Integral> v)
{
	return details::input_base_number<base>(in,v.reference);
}

}