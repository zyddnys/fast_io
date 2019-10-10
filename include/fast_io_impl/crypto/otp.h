#pragma once

#include<random>

namespace fast_io::crypto
{

template<standard_output_stream T,typename strT = std::basic_string<typename T::char_type>>
class oone_time_pad
{
	std::random_device device;
	T& t;
public:
	using char_type = typename T::char_type;
	using key_type = strT;
private:
	std::uniform_int_distribution<char_type> dis;
	key_type string;
public:
	oone_time_pad(T& t):t(t),dis(std::numeric_limits<char_type>::min(),std::numeric_limits<char_type>::max()){}
	void put(char_type ch)
	{
		char_type c = dis(device);
		string.push_back(c);
		t.put(c^ch);
	}
	template<typename ContiguousIterator>
	void write(ContiguousIterator b,ContiguousIterator e)
	{
		write_precondition<char_type>(b, e);
        auto pb(static_cast<char_type const*>(static_cast<void const*>(std::addressof(*b))));
		auto pi(pb), pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));
		for(;pi!=pe;++pi)
			put(*pi);
	}
	void flush(){t.flush();}
	auto& key()  const {return string;}
	auto& key() {return string;}
};

template<standard_input_stream T,typename strv = std::basic_string_view<typename T::char_type>>
class ione_time_pad
{
	T& t;
public:
	using char_type = typename T::char_type;
	using key_type = strv;
private:
	fast_io::basic_istring_view<key_type> istr;
public:
	template<typename ...Args>
	ione_time_pad(T& t,Args&& ...args):t(t),istr(std::forward<Args>(args)...){}
	char_type get()
	{
		return t.get()^istr.get();
	}
	std::pair<char_type,bool> try_get()
	{
		auto ch(t.try_get());
		if(ch.second)
			return {0,true};
		return {ch.first^istr.get(),false};
	}
	template<typename Contiguous_iterator>
	Contiguous_iterator read(Contiguous_iterator b,Contiguous_iterator e)
		requires standard_input_stream<T>
	{
		auto pb(static_cast<char_type*>(static_cast<void*>(std::addressof(*b))));
		auto pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));
		auto pi(pb);
		for(;pi!=pe;++pi)
		{
			auto ch(try_get());
			if(ch.second)
				break;
			*pi=ch.first;
		}
		return b+(pi-pb)*sizeof(char_type)/sizeof(*b);
	}
};

}
