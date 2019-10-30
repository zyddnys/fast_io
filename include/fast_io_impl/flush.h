#pragma once

namespace fast_io
{

template<input_stream Ihandler>
requires (!character_input_stream<Ihandler>)
class nobuf_reader:public Ihandler
{
public:
	template<typename... Args>
	requires std::constructible_from<Ihandler,Args...>
	constexpr nobuf_reader(Args&&... args) :Ihandler(std::forward<Args>(args)...) {}
	using char_type = typename Ihandler::char_type;
};

template<input_stream Ihandler>
inline constexpr auto get(nobuf_reader<Ihandler>& ib)
{
	typename Ihandler::char_type ch;
	auto address(std::addressof(ch));
	if(reads(ib,address,address+1)==address)
		throw eof();
	return ch;
}

template<input_stream Ihandler>
inline constexpr std::pair<typename Ihandler::char_type,bool> try_get(nobuf_reader<Ihandler>& ib)
{
	typename Ihandler::char_type ch;
	auto address(std::addressof(ch));
	if(reads(ib,address,address+1)==address)
		return {0,true};
	return {ch,false};
}

template<output_stream Ohandler>
requires (!character_output_stream<Ohandler>)
class nobuf_writer :public Ohandler
{
public:
	using char_type = typename Ohandler::char_type;
	template<typename... Args>
	requires std::constructible_from<Ohandler,Args...>
	constexpr nobuf_writer(Args&&... args) :Ohandler(std::forward<Args>(args)...) {}
};

template<output_stream Ohandler>
inline constexpr void put(nobuf_writer<Ohandler>& ob,typename Ohandler::char_type ch)
{
	auto address(std::addressof(ch));
	writes(ob,address, address + 1);
}

template<output_stream Ohandler>
class immediately_flush:public Ohandler
{
public:
	using char_type = typename Ohandler::char_type;
	template<typename... Args>
	requires std::constructible_from<Ohandler,Args...>
	constexpr immediately_flush(Args&&... args):Ohandler(std::forward<Args>(args)...){}
};

template<output_stream Ohandler,std::contiguous_iterator Iter>
inline constexpr void writes(immediately_flush<Ohandler>& ob,Iter cbegin,Iter cend)
{
	writes(static_cast<Ohandler&>(ob),cbegin,cend);
	flush(ob);
}

template<output_stream Ohandler>
inline constexpr void put(immediately_flush<Ohandler>& ob,typename Ohandler::char_type ch)
{
	auto const address(std::addressof(ch));
	writes(ob,address,address+1);
}

template<character_output_stream Ohandler>
inline constexpr void put(immediately_flush<Ohandler>& ob,typename Ohandler::char_type ch)
{
	put(static_cast<Ohandler&>(ob),ch);
	flush(ob);
}


template<output_stream Ohandler,typename Ohandler::char_type flush_character>
class char_flush:public Ohandler
{
public:
	using char_type = typename Ohandler::char_type;
	template<typename... Args>
	requires std::constructible_from<Ohandler,Args...>
	constexpr char_flush(Args&&... args):Ohandler(std::forward<Args>(args)...){}
};

template<output_stream Ohandler,typename Ohandler::char_type flush_character,std::contiguous_iterator Iter>
inline constexpr void writes(char_flush<Ohandler,flush_character>& ob,Iter b,Iter e)
{
	using char_type = typename Ohandler::char_type;
	writes_precondition<char_type>(b,e);
	auto pb(std::to_address(b)),pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));
	for(auto pi(pb);pi!=pe;++pi)
		if(*pi==flush_character)
		{
			writes(static_cast<Ohandler&>(ob),pb,pi+1);
			flush(ob);
			pb=pi+1;
		}
	writes(static_cast<Ohandler&>(ob),pb,pe);
}

template<character_output_stream Ohandler,typename Ohandler::char_type flush_character>
inline constexpr void put(char_flush<Ohandler,flush_character>& ob,typename Ohandler::char_type ch)
{
	put(static_cast<Ohandler&>(ob),ch);
	if(ch==flush_character)
		flush(ob);
}

template<typename T>
using line_flush = char_flush<T,'\n'>;

}
