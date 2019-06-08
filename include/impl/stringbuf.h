#ifndef FAST_IO_STRINGBUF_H
#define FAST_IO_STRINGBUF_H

#include<string>
#include"concept.h"
#include<string_view>

namespace fast_io
{
	template< typename T>
	class basic_ibuf_string_view
	{
		T s;
	public:
		using value_type = T;
		using traits_type = typename value_type::traits_type;
		using int_type = typename traits_type::int_type;
		using char_type = typename T::value_type;
	public:
		template<typename ...Args>
		basic_ibuf_string_view(Args&& ...args):s(std::forward<Args>(args)...){}
		auto& str()
		{
			return s;
		}
		bool eof() const
		{
			return s.empty();
		}
		operator bool() const
		{
			return !s.empty();
		}
		template<Pointer pointer>
		pointer read(pointer begin,pointer end)
		{
			std::size_t const cped(s.copy(begin,end-begin));
			s.remove_prefix(cped);
			return begin+cped;
		}
		int_type get()
		{
			if(eof())
				return traits_type::eof();
			int_type ch(s.front());
			s.remove_prefix(1);
			return ch;
		}
	};
	
	template< typename T>
	class basic_obuf_string
	{
		T s;
	public:
		using value_type = T;
		using traits_type = typename value_type::traits_type;
		using int_type = typename traits_type::int_type;
		using char_type = typename T::value_type;
		auto& str()
		{
			return s;
		}
		template<typename contiguous_iterator>
		void write(contiguous_iterator cbegin,contiguous_iterator cend)
		{
			s.append(reinterpret_cast<char_type const*>(cbegin),reinterpret_cast<char_type const*>(cend));
		}
		template<typename V>
		void put(V ch)
		{
			s.push_back(ch);
		}
		void flush(){}
	};
}

#endif