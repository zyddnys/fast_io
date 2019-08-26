#pragma once

namespace fast_io
{

namespace details
{
template<typename T>
struct hex_t				//base 16
{
	T& reference;
};

template<typename T>
struct hexupper_t				//base 16 uppercase
{
	T& reference;
};

template<Integral T>
struct oct_t			//base 8
{
	T& reference;
};

template<Integral T>
struct bin_t			//base 2
{
	T& reference;
};

}

template<typename T>
inline constexpr details::hex_t<T> hex(T& t)
{
	return {t};
}
template<typename T>
inline constexpr details::hex_t<T const> hex(T const& t)
{
	return {t};
}

template<typename T>
inline constexpr details::hexupper_t<T> hexupper(T& t)
{
	return {t};
}
template<typename T>
inline constexpr details::hexupper_t<T const> hexupper(T const& t)
{
	return {t};
}

template<typename T>
inline constexpr details::oct_t<T> oct(T& t)
{
	return {t};
}
template<typename T>
inline constexpr details::oct_t<T const> oct(T const& t)
{
	return {t};
}

template<typename T>
inline constexpr details::bin_t<T> bin(T& t)
{
	return {t};
}

template<typename T>
inline constexpr details::bin_t<T const> bin(T const& t)
{
	return {t};
}
/*
namespace details
{
template<bool uppercase,standard_output_stream output>
inline output& output_hex(output& out,Unsigned_integer a)
{
//upper: 65 :A 70: F
//lower: 97 :a 102 :f
	if(a)
	{
		std::array<typename output::char_type,sizeof(a)*8> v;
		auto ed(v.end());
		--ed;
		auto temp(a%16);
		if(temp<10)
			*ed = temp+48;
		else
		{
			if constexpr (uppercase)
				*ed = temp+55;	
			else
				*ed = temp+87;
		}
		while(a/=16)
		{
			--ed;
			auto temp(a%16);
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
		out.write(ed,v.end());
	}
	else
		out.put(48);
	return out;
}

template<standard_output_stream output>
inline output& output_bin(output& out,Unsigned_integer a)
{
	return out;
}
}*/

template<standard_output_stream output>
inline constexpr output& operator<<(output& out,details::hex_t<Unsigned_integer> v)
{
	return details::output_hex<false>(out,v.reference);
}

template<standard_output_stream output,Signed_integer T>
inline constexpr output& operator<<(output& out,details::hex_t<T> v)
{
	auto a(v.reference);
	if(a<0)
	{
		out.put('-');
		a=-a;
	}
	return details::output_hex<false>(out,static_cast<std::make_unsigned_t<decltype(a)>>(a));
}

template<standard_output_stream output>
inline constexpr output& operator<<(output& out,details::hexupper_t<Unsigned_integer> v)
{
	return out<<details::output_unsigned_oct_number<16,true>(out,v.reference));
}

template<standard_output_stream output,Signed_integer T>
inline constexpr output& operator<<(output& out,details::hexupper_t<T> v)
{
	return out<<details::output_unsigned_oct_number<16,false>(out,v.reference));
}

template<standard_output_stream output>
inline constexpr output& operator<<(standard_output_stream& out,details::oct_t<Unsigned_integer> v)
{
	return out<<details::output_unsigned_oct_number<8,false>(out,v.reference));
}

template<standard_output_stream output,Signed_integer T>
inline constexpr output& operator<<(output& out,details::oct_t<T> v)
{
	return out<<details::output_signed_oct_number<8,false>(out,v.reference));
}

template<standard_output_stream output>
inline constexpr output& operator<<(output& out,details::bin_t<Unsigned_integer> v)
{
	return out<<details::output_unsigned_base_number<2,false>(out,v.reference));
}

template<standard_output_stream output>
inline constexpr output& operator<<(output& out,details::bin_t<Signed_integer> v)
{
	return out<<details::output_signed_base_number<2,false>(out,v.reference));
}

}