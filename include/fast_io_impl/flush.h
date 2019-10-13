#pragma once

namespace fast_io
{

template<input_stream Ihandler>
class nobuf_reader:public Ihandler
{
public:
	template<typename... Args>
	requires std::constructible_from<Ihandler,Args...>
	constexpr nobuf_reader(Args&&... args) :Ihandler(std::forward<Args>(args)...) {}
	using char_type = typename Ihandler::char_type;
	char_type get()
	{
		char_type ch;
		auto address(std::addressof(ch));
		if(Ihandler::reads(address,address+1)==address)
			throw eof();
		return ch;
	}
	std::pair<char_type,bool> try_get()
	{
		char_type ch;
		auto address(std::addressof(ch));
		if(Ihandler::reads(address,address+1)==address)
			return {0,true};
		return {ch,false};
	}
};

template<stream Ohandler>
class nobuf_writer :public Ohandler
{
public:
	using char_type = typename Ohandler::char_type;
	template<typename... Args>
	requires std::constructible_from<Ohandler,Args...>
	constexpr nobuf_writer(Args&&... args) :Ohandler(std::forward<Args>(args)...) {}
	template<std::contiguous_iterator Iter>
	constexpr void writes(Iter cbegin, Iter cend) requires output_stream<Ohandler>
	{
		Ohandler::writes(cbegin, cend);
	}
	constexpr void put(char_type ch) requires standard_output_stream<Ohandler>
	{
		Ohandler::put(ch);
	}
	constexpr void put(char_type ch) requires output_stream<Ohandler>
	{
		auto address(std::addressof(ch));
		Ohandler::writes(address, address + 1);
	}
};

template<stream Ohandler>
class immediately_flush:public Ohandler
{
public:
	using char_type = typename Ohandler::char_type;
	template<typename... Args>
	requires std::constructible_from<Ohandler,Args...>
	constexpr immediately_flush(Args&&... args):Ohandler(std::forward<Args>(args)...){}
	template<std::contiguous_iterator Iter>
	constexpr void writes(Iter cbegin,Iter cend) requires output_stream<Ohandler>
	{
		Ohandler::writes(cbegin,cend);
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
		Ohandler::writes(address,address+1);
		Ohandler::flush();
	}
};

template<output_stream Ohandler,typename Ohandler::char_type flush_character>
class char_flush:public Ohandler
{
public:
	using char_type = typename Ohandler::char_type;
	template<typename... Args>
	requires std::constructible_from<Ohandler,Args...>
	constexpr char_flush(Args&&... args):Ohandler(std::forward<Args>(args)...){}
	template<std::contiguous_iterator Iter>
	constexpr void writes(Iter b,Iter e)
	{
		writes_precondition<char_type>(b,e);
		auto pb(std::to_address(b)),pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));
		for(auto pi(pb);pi!=pe;++pi)
			if(*pi==flush_character)
			{
				Ohandler::writes(pb,pi+1);
				Ohandler::flush();
				pb=pi+1;
			}
		Ohandler::writes(pb,pe);
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
		Ohandler::writes(address,address+1);
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
