#pragma once

#include<random>

namespace fast_io::crypto
{

template<standard_output_stream T,typename strT = std::basic_string_view<typename T::char_type>>
class oone_time_pad
{
public:
	using char_type = typename T::char_type;
	using key_type = strT;
private:
	key_type strvw;
	T t;
	typename key_type::iterator iter;
	void write_remain()
	{
		if(iter!=strvw.cend())
			t.write(iter,strvw.cend());
	}
public:
    template<typename T1, typename ...Args>
	requires std::constructible_from<key_type, strT>&&std::constructible_from<T, Args...>
	oone_time_pad(T1&& t1,Args&& ...args):strvw(std::forward<T1>(t1)),t(std::forward<Args>(args)...),iter(strvw.cbegin()){}
	void put(char_type ch)
	{
		if(iter==strvw.cend())
			throw std::runtime_error("key is too short for one time pad");
		t.put(*iter^ch);
		++iter;
	}
	template<std::contiguous_iterator Iter>
	void write(Iter b,Iter e)
	{
		write_precondition<char_type>(b, e);
        auto pb(static_cast<char_type const*>(static_cast<void const*>(std::addressof(*b))));
		auto pi(pb), pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));
		for(;pi!=pe;++pi)
			put(*pi);
	}
	void flush()
	{
		write_remain();
		iter=strvw.cend();
		t.flush();
	}
	auto& key()  const {return strvw;}
	auto& key() {return strvw;}
	oone_time_pad(oone_time_pad const&) = delete;
	oone_time_pad& operator=(oone_time_pad const&) = delete;
	oone_time_pad(oone_time_pad&& o) noexcept:strvw(std::move(strvw)),t(std::move(o.t)),iter(std::move(o.iter)){}
	oone_time_pad& operator=(oone_time_pad&& o) noexcept
	{
		if(std::addressof(o)!=this)
		{
			try
			{
				write_remain();
			}
			catch(...){}
			strvw=std::move(o.strvw);
			t=std::move(o.t);
			iter=std::move(o.iter);
		}
		return *this;
	}
	~oone_time_pad()
	try
	{
		write_remain();
	}
	catch(...)
	{
	}
};

template<standard_input_stream T,typename strv = std::basic_string_view<typename T::char_type>>
class ione_time_pad
{
	fast_io::basic_istring_view<strv> istr;
	T t;
public:
	using char_type = typename T::char_type;
	using key_type = strv;
	template<typename T1,typename ...Args>
	requires std::constructible_from<key_type, T1>&&std::constructible_from<T, Args...>
	ione_time_pad(T1&& t1,Args&& ...args):istr(std::forward<T1>(t1)),t(std::forward<Args>(args)...){}
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
	template<std::contiguous_iterator Iter>
	Iter read(Iter b,Iter e)
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
	auto& key_istrview() {return istr;}
};

}
