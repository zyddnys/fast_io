#pragma once

namespace fast_io
{
template<typename T>
class stream_view
{
	T& strm;
public:
	using char_type = typename T::char_type;
	using traits_type = typename T::traits_type;
	stream_view(T& ref):strm(ref){}
	template<typename Contiguous_iterator>
	Contiguous_iterator read(Contiguous_iterator begin,Contiguous_iterator end) requires fast_io::details::input_stream_impl<T>
	{
		return begin+(strm.sgetn(static_cast<char_type*>(static_cast<void*>(std::addressof(*begin))),(end-begin)*sizeof(*begin)/sizeof(char_type))*sizeof(char_type)/sizeof(*begin));
	}
	template<typename Contiguous_iterator>
	void write(Contiguous_iterator begin,Contiguous_iterator end) requires fast_io::details::output_stream_impl<T>
	{
		write_precondition<char_type>(begin,end);
		if(!strm.write(static_cast<char_type const*>(static_cast<void const*>(std::addressof(*begin))),(end-begin)*sizeof(*begin)/sizeof(char_type)))
			throw std::runtime_error("write failed for stream view");
	}
	char_type get() requires fast_io::details::input_stream_impl<T>
	{
		auto ch(strm.get());
		if(ch==traits_type::eof())
			throw std::runtime_error("try to get() from EOF stream view");
		return traits_type::to_char_type(ch);
	}
	std::pair<char_type,bool> try_get() requires fast_io::details::input_stream_impl<T>
	{
		auto ch(strm.get());
		if(ch==traits_type::eof())
			return {0,true};
		return {traits_type::to_char_type(ch),false};
	}
	void put(char_type ch) requires fast_io::details::output_stream_impl<T>
	{
		if(!strm.put(ch))
			throw std::runtime_error("put() failed for streambuf view");
	}
	void flush() requires fast_io::details::output_stream_impl<T>
	{
		strm.flush();
	}
};

template<typename stream>
stream_view(stream&) -> stream_view<stream>;

}