#pragma once
#include"concept.h"
#include"precondition.h"

namespace fast_io
{
template<output_stream Ohandler>
class immediately_flush:public Ohandler
{
public:
	using char_type = typename Ohandler::char_type;
	template<typename... Args>
	constexpr immediately_flush(Args&&... args):Ohandler(std::forward<Args>(args)...){}
	template<typename Contiguous_Iterator>
	constexpr void write(Contiguous_Iterator cbegin,Contiguous_Iterator cend)
	{
		Ohandler::write(cbegin,cend);
		Ohandler::flush();
	}
	constexpr void put(char_type ch) requires standard_output_stream<Ohandler>
	{
		Ohandler::put(ch);
		Ohandler::flush();
	}
	constexpr auto& native_handle()
	{
		return Ohandler::native_handle();
	}
};

template<output_stream Ohandler,typename Ohandler::char_type flush_character>
class char_flush:public Ohandler
{
public:
	using char_type = typename Ohandler::char_type;
	template<typename... Args>
	constexpr char_flush(Args&&... args):Ohandler(std::forward<Args>(args)...){}
	template<typename Contiguous_Iterator>
	constexpr void write(Contiguous_Iterator b,Contiguous_Iterator e)
	{
		write_precondition<char_type>(b,e);
		auto pb(std::addressof(*b)),pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));
		for(auto pi(pb);pi!=pe;++pi)
			if(*pi==flush_character)
			{
				Ohandler::write(pb,pi+1);
				Ohandler::flush();
				pb=pi+1;
			}
		Ohandler::write(pb,pe);
	}
	constexpr void put(char_type ch) requires standard_output_stream<Ohandler>
	{
		Ohandler::put(ch);
		if(ch==flush_character)
			Ohandler::flush();
	}
	constexpr auto& native_handle()
	{
		return Ohandler::native_handle();
	}
};


template<typename T>
using line_flush = char_flush<T,'\n'>;

}
