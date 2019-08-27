#pragma once
#include"concept.h"

namespace fast_io
{
template<output_stream Ohandler>
class immediately_flush
{
	Ohandler oh;
public:
	using native_handle_type = Ohandler;
	using char_type = typename native_handle_type::char_type;
	template<typename... Args>
	constexpr immediately_flush(Args&&... args):oh(std::forward<Args>(args)...){}
	constexpr void flush()
	{
		oh.flush();
	}
	template<typename Contiguous_Iterator>
	constexpr void write(Contiguous_Iterator cbegin,Contiguous_Iterator cend)
	{
		oh.write(cbegin,cend);
		oh.flush();
	}
	constexpr void put(char_type ch) requires !standard_output_stream<native_handle_type>
	{
		auto const address(std::addressof(ch));
		oh.write(address,address+1);
		oh.flush();
	}
	constexpr void put(char_type ch) requires standard_output_stream<native_handle_type>
	{
		oh.put(ch);
		oh.flush();
	}
	constexpr auto& native_handle()
	{
		return oh;
	}
};
}
