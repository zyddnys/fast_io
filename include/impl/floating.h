#pragma once

namespace fast_io
{
	
namespace details
{
template<std::size_t v>
inline auto constexpr compiler_time_10_exp_calculation()
{
	std::array<std::uint64_t,v> a{1};
	for(std::size_t i(1);i<a.size();++i)
		a[i]=a[i-1]*10;
	return a;
}

inline auto constexpr exp10_array(compiler_time_10_exp_calculation<20>());
inline auto constexpr log2_minus_10(-std::log2(10));

}

template<Floating_point T>
inline void print(standard_output_stream& out,details::fixed<T const> a)
{
	auto e(a.reference);
	if(e<0)
	{
		e=-e;
		out.put('-');
	}
	std::uint_fast64_t u(e);
	print(out,u);
	e-=u;
	if(a.precision)
	{
		out.put('.');
		auto p(static_cast<decltype(u)>(e*details::exp10_array.at(a.precision+1)));
		auto md(p%10),pt(p/10);
		if(md<5||(md==5&&!(pt&1)))
			print(out,setw(a.precision,pt,'0'));
		else
			print(out,setw(a.precision,pt+1,'0'));
	}
}

template<Floating_point T>
inline void print(standard_output_stream& out,details::scientific<T const> a)
{
	auto e(a.reference);
	if(e<0)
	{
		e=-e;
		out.put('-');
	}
	auto x(std::floor(std::log10(e)));
	if(0<x)
		++x;
	print(out,fixed(e*std::exp2(x*details::log2_minus_10),a.precision));
	if(x==0)
		return;
	out.put('e');
	if(x<0)
	{
		out.put('-');
		print(out,static_cast<std::uint64_t>(-x));
	}
	print(out,static_cast<std::uint64_t>(x));
}

template<standard_output_stream output,Floating_point T>
inline void print(output& out,details::floating_point_default<T const> a)
{
	auto e(a.reference);
	if(e<0)
	{
		e=-e;
		out.put('-');
	}
	auto x(std::floor(std::log10(e)));
	if(0<x)
		++x;
	{
	auto fix(std::fabs(x)<=a.precision);
	basic_ostring<std::basic_string<typename output::char_type>> bas;
	if(fix)
		print(bas,fixed(e,a.precision));
	else
		print(bas,fixed(e*std::exp2(x*details::log2_minus_10),a.precision));
	auto& str(bas.str());
	if(str.find('.')!=std::string::npos)
	{
		for(;!str.empty()&&str.back()=='0';str.pop_back());
		if(!str.empty()&&str.back()=='.')
			str.pop_back();
		print(out,str);
	}
	if(fix)
		return;
	}
	if(x==0)
		return;
	out.put('e');
	if(x<0)
	{
		out.put('-');
		x=-x;
	}
	print(out,static_cast<std::uint64_t>(x));
}

template<Floating_point T>
inline void print(standard_output_stream &output,T const& p)
{
	print(output,floating_point_default(p,3));
}

template<standard_input_stream input>
inline constexpr void scan(input& in,Floating_point &t)
{
/*	bool negative(false);
	bool has_point(false);
	while(true)
	{
		auto ch(in.get());
		if((48<=ch&&ch<=57))
		{
			t=ch-48;
			break;
		}
		else if(ch=='-')
		{
			t=0;
			negative=true;
			break;
		}
		else if(ch=='.')
		{
			t=0;
			has_point=true;
			break;
		}
	}
	if(!has_point)
	{
		while(true)
		{
			auto ch(in.try_get());
			if(ch.second)
				break;
			else if((48<=ch.first&&ch.first<=57))
			{
				t=t*10+ch.first-48;
				break;
			}
			else if(ch.first=='E'||ch.first=='e')
		}
	}
	if(negative)
		t=-t;*/
}

}
