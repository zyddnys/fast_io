#pragma once

namespace fast_io
{
	
namespace details
{

template<typename>
struct text_view_interal_variable{};

template<standard_input_stream T>
struct text_view_interal_variable<T>
{
	bool state=false;
	typename T::char_type internal_character = 0;
};

}

template<typename T>
requires standard_input_stream<T>||standard_output_stream<T>
class text_view
{
	T& ib;
	details::text_view_interal_variable<T> state;
public:
	using native_interface_t = T;
	using char_type = typename native_interface_t::char_type;
public:
	constexpr text_view(T& ibv):ib(ibv){}
	constexpr auto& native_handle()
	{
		return ib;
	}
	constexpr char_type get() requires standard_input_stream<T>
	{
		if(state.state)
		{
			state.state=false;
			return state.internal_character;
		}
		auto ch(ib.get());
		if(ch=='\r')
		{
			auto internal(ib.try_get());
			if(internal.second)
				return ch;
			if(internal.first=='\n')
				return '\n';
			state.state=true;
			state.internal_character=internal.first;
			return ch;
		}
		else
			return ch;
	}
	constexpr std::pair<char_type,bool> try_get() requires standard_input_stream<T>
	{
		if(state.state)
		{
			state.state=false;
			return {state.internal_character,false};
		}
		auto ch(ib.try_get());
		if(ch.second)
			return {0,true};
		if(ch.first=='\r')
		{
			auto internal(ib.try_get());
			if(internal.second)
				return ch;
			if(internal.first=='\n')
				return internal;
			state.state=true;
			state.internal_character=internal.first;
			return ch;
		}
		else
			return ch;
	}
	template<typename Contiguous_iterator>
	constexpr Contiguous_iterator read(Contiguous_iterator b,Contiguous_iterator e)
		requires standard_input_stream<T>
	{
		auto pb(static_cast<char_type*>(static_cast<void*>(std::addressof(*b))));
		auto pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));
		auto pi(pb);
		for(;pi!=pe;++pi)
			*pi=get();
		return b+(pi-pb)*sizeof(char_type)/sizeof(*b);
	}
	constexpr void put(char_type ch)	requires standard_output_stream<T>
	{
#if defined(__WINNT__) || defined(_MSC_VER)
		if(ch=='\n')
			ib.put('\r');
#endif
		ib.put(ch);
	}
	template<typename Contiguous_iterator>
	constexpr void write(Contiguous_iterator b,Contiguous_iterator e)
		requires standard_output_stream<T>
	{
#if defined(__WINNT__) || defined(_MSC_VER)
		write_precondition<char_type>(b,e);
		auto pb(static_cast<char_type const*>(static_cast<void const*>(std::addressof(*b))));
		auto last(pb);
		auto pi(pb),pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));
		for(;pi!=pe;++pi)
			if(*pi=='\n')
			{
				if(last!=pi)
					ib.write(last,pi-1);
				ib.put('\r');
				ib.put('\n');
				last=pi+1;
			}
		ib.write(last,pe);
#else
		ib.write(b,e);
#endif
	}
	constexpr void flush() requires standard_output_stream<T>
	{
		ib.flush();
	}
};

template<stream srm>
text_view(srm&) -> text_view<srm>;

}