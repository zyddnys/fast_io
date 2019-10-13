#pragma once

namespace fast_io
{
	
namespace stream_view_details
{
template<typename T>
concept istream_concept_impl = requires(T& in)
{
	in.operator>>;
};

template<typename T>
concept ostream_concept_impl = requires(T& out)
{
	out.operator<<;
};
}
	
template<typename T>
class stream_view
{
	T strm;
public:
	using char_type = typename T::char_type;
	using traits_type = typename T::traits_type;
	template<typename ...Args>
	requires std::constructible_from<T,Args...>
	stream_view(Args&& ...args):strm(std::forward<Args>(args)...){}
	template<std::contiguous_iterator Iter>
	Iter reads(Iter begin,Iter end) requires fast_io::stream_view_details::istream_concept_impl<T>
	{
		return begin+(strm.sgetn(static_cast<char_type*>(static_cast<void*>(std::to_address(begin))),(end-begin)*sizeof(*begin)/sizeof(char_type))*sizeof(char_type)/sizeof(*begin));
	}
	template<std::contiguous_iterator Iter>
	void writes(Iter begin,Iter end) requires fast_io::stream_view_details::ostream_concept_impl<T>
	{
		writes_precondition<char_type>(begin,end);
		if(!strm.write(static_cast<char_type const*>(static_cast<void const*>(std::to_address(begin))),(end-begin)*sizeof(*begin)/sizeof(char_type)))
			throw std::runtime_error("writes failed for stream view");
	}
	char_type get() requires fast_io::stream_view_details::istream_concept_impl<T>
	{
		auto ch(strm.get());
		if(ch==traits_type::eof())
			throw std::runtime_error("try to get() from EOF stream view");
		return traits_type::to_char_type(ch);
	}
	std::pair<char_type,bool> try_get() requires fast_io::stream_view_details::istream_concept_impl<T>
	{
		auto ch(strm.get());
		if(ch==traits_type::eof())
			return {0,true};
		return {traits_type::to_char_type(ch),false};
	}
	void put(char_type ch) requires fast_io::stream_view_details::ostream_concept_impl<T>
	{
		if(!strm.put(ch))
			throw std::runtime_error("put() failed for streambuf view");
	}
	auto& native_handle() { return strm;}
	void flush() requires fast_io::stream_view_details::ostream_concept_impl<T>
	{
		strm.flush();
	}
};

template<typename stream>
stream_view(stream&&) noexcept -> stream_view<stream>;

}