#pragma once
#include<stdexcept>

namespace fast_io
{
template<typename char_type,typename Contiguous_iterator>
inline constexpr void write_precondition(Contiguous_iterator b,Contiguous_iterator e)
{
	if constexpr(sizeof(*b)%sizeof(char_type))
		if((e-b)*sizeof(*b)%sizeof(char_type))
			throw std::runtime_error("incorrect write size");
}

}