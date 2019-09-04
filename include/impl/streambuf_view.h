#pragma once

namespace fast_io
{
template<typename T>
class streambuf_view
{
	T& rdbuf;
public:
	using char_type = typename T::char_type;
	using traits_type = typename T::traits_type;
	streambuf_view(T* bufpointer):rdbuf(*bufpointer){}
	template<typename Contiguous_iterator>
	Contiguous_iterator read(Contiguous_iterator begin,Contiguous_iterator end)
	{
		return begin+(rdbuf.sgetn(static_cast<char_type*>(static_cast<void*>(std::addressof(*begin))),(end-begin)*sizeof(*begin)/sizeof(char_type))*sizeof(char_type)/sizeof(*begin));
	}
	template<typename Contiguous_iterator>
	void write(Contiguous_iterator begin,Contiguous_iterator end)
	{
		write_precondition<char_type>(begin,end);
		decltype(rdbuf.sputn(nullptr,0)) write_bytes((end-begin)*sizeof(*begin)/sizeof(char_type));
		if(rdbuf.sputn(static_cast<char_type const*>(static_cast<void const*>(std::addressof(*begin))),write_bytes)<write_bytes)
			throw std::runtime_error("write() failed for streambuf view");
	}
	char_type get()
	{
		auto ch(rdbuf.sbumpc());
		if(ch==traits_type::eof())
			throw std::runtime_error("try to get() from EOF streambuf view");
		return traits_type::to_char_type(ch);
	}
	std::pair<char_type,bool> try_get()
	{
		auto ch(rdbuf.sbumpc());
		if(ch==traits_type::eof())
			return {0,true};
		return {traits_type::to_char_type(ch),false};
	}
	void put(char_type ch)
	{
		if(rdbuf.sputc(ch)==traits_type::eof())
			throw std::runtime_error("put() failed for streambuf view");
	}
	void flush()
	{
		rdbuf.sync();
	}
};

template<typename stream>
streambuf_view(stream*) -> streambuf_view<stream>;

}