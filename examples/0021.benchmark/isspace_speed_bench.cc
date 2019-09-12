#include<array>
#include<cctype>
#include<iostream>
#include"timer.h"
template<typename T>
inline constexpr bool misspace1(T const& e)
{
	return e==' '||e=='\t'||e=='\n'||e=='\v'||e=='\f'||e=='\r';
}

template<typename T>
inline constexpr bool misspace2(T const& t)
{
	std::make_unsigned_t<T> e(t);
	return e==' '||e=='\t'||e=='\n'||e=='\v'||e=='\f'||e=='\r';
}

template<typename T>
inline constexpr bool misspace3(T const& e)
{
	switch(e)
	{
		case ' ':case'\t':case '\n':case '\v':case '\f':case '\r': return true;
	}
	return false;
}

template<typename T>
inline constexpr bool misspace4(T const& t)
{
	std::make_unsigned_t<T> e(t);
	switch(e)
	{
		case ' ':case'\t':case '\n':case '\v':case '\f':case '\r': return true;
	}
	return false;
}

template<typename T>
inline constexpr bool fastisspace(T ch)
{
	return (9<=ch&&ch<=13)||ch==0x20;
}

template<typename T>
inline constexpr bool fastisspacerev(T ch)
{
	return ch==0x20||(9<=ch&&ch<=13);
}

template<typename T>
inline constexpr bool fastisspacerev2(T ch)
{
	if(ch==0x20)
		return true;
	std::make_unsigned_t<T> e(ch);
	e-=9;
	return e<5;
}

template<typename T>
inline constexpr bool fastisspacerev3(T ch)
{
	switch(ch)
	{
		case 20: return true;
		default:
			std::make_unsigned_t<T> e(ch);
			e-=9;
			return e<5;
	}
	
}
int main()
{
	std::array<char,4> a={'a','e','f',105};
	std::size_t constexpr N(100000000);
	std::size_t v(0);
/*	{
		cqw::timer t("std::isspace");
		for(std::size_t i(0);i!=N;++i)
		{
			for(auto const& e : a)
				v+=std::isspace(e)|std::isspace(e+1)|std::isspace(e+2)|std::isspace(e+3);
		}
	}*/
	
	static_assert(fastisspacerev2(' '),"false");
	{
		cqw::timer t("misspace1");
		for(std::size_t i(0);i!=N;++i)
		{
			for(auto const& e : a)
				v+=misspace1(e)|misspace1(e+1)|misspace1(e+2)|misspace1(e+3);
		}
	}
	{
		cqw::timer t("misspace2");
		for(std::size_t i(0);i!=N;++i)
		{
			for(auto const& e : a)
				v+=misspace2(e)|misspace2(e+1)|misspace2(e+2)|misspace2(e+3);
		}
	}
	{
		cqw::timer t("misspace3");
		for(std::size_t i(0);i!=N;++i)
		{
			for(auto const& e : a)
				v+=misspace3(e)|misspace3(e+1)|misspace3(e+2)|misspace3(e+3);
		}
	}
	{
		cqw::timer t("misspace4");
		for(std::size_t i(0);i!=N;++i)
		{
			for(auto const& e : a)
				v+=misspace4(e)|misspace4(e+1)|misspace4(e+2)|misspace4(e+3);
		}
	}
	{
		cqw::timer t("fastisspace");
		for(std::size_t i(0);i!=N;++i)
		{
			for(auto const& e : a)
				v+=fastisspace(e)|fastisspace(e+1)|fastisspace(e+2)|fastisspace(e+3);
		}
	}
	{
		cqw::timer t("fastisspacerev");
		for(std::size_t i(0);i!=N;++i)
		{
			for(auto const& e : a)
				v+=fastisspacerev(e)|fastisspacerev(e+1)|fastisspacerev(e+2)|fastisspacerev(e+3);
		}
	}
	{
		cqw::timer t("fastisspacerev2");
		for(std::size_t i(0);i!=N;++i)
		{
			for(auto const& e : a)
				v+=fastisspacerev2(e)|fastisspacerev2(e+1)|fastisspacerev2(e+2)|fastisspacerev2(e+3);
		}
	}
	{
		cqw::timer t("fastisspacerev3");
		for(std::size_t i(0);i!=N;++i)
		{
			for(auto const& e : a)
				v+=fastisspacerev3(e)|fastisspacerev3(e+1)|fastisspacerev3(e+2)|fastisspacerev3(e+3);
		}
	}
	std::cout<<v<<'\n';
}