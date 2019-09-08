#pragma once
#include"concept.h"
#include"precondition.h"
#include<utility>

namespace fast_io
{

template<input_stream Ihandler>
class nobuf_reader:public Ihandler
{
public:
	using char_type = typename Ihandler::char_type;
	char_type get() requires !standard_input_stream<Ihandler>
	{
		char_type ch;
		auto address(std::addressof(ch));
		if(Ihandler::read(address,address+1)==address)
			throw eof();
		return ch;
	}
	std::pair<char_type,bool> try_get() requires !standard_input_stream<Ihandler>
	{
		char_type ch;
		auto address(std::addressof(ch));
		if(Ihandler::read(address,address+1)==address)
			return {0,true};
		return {ch,false};
	}
};

template<stream Ohandler>
class immediately_flush:public Ohandler
{
public:
	using char_type = typename Ohandler::char_type;
	template<typename... Args>
	constexpr immediately_flush(Args&&... args):Ohandler(std::forward<Args>(args)...){}
	template<typename Contiguous_Iterator>
	constexpr void write(Contiguous_Iterator cbegin,Contiguous_Iterator cend) requires output_stream<Ohandler>
	{
		Ohandler::write(cbegin,cend);
		Ohandler::flush();
	}
	constexpr void put(char_type ch) requires standard_output_stream<Ohandler>
	{
		Ohandler::put(ch);
		Ohandler::flush();
	}
	constexpr void put(char_type ch) requires output_stream<Ohandler>
	{
		auto address(std::addressof(ch));
		Ohandler::write(address,address+1);
		Ohandler::flush();
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
	constexpr void put(char_type ch)
	{
		auto address(std::addressof(ch));
		Ohandler::write(address,address+1);
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
