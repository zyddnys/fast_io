#pragma once
#include"concept.h"
#include<stdexcept>

namespace fast_io
{
template<typename char_type,typename Contiguous_iterator>
inline constexpr void write_precondition(Contiguous_iterator b,Contiguous_iterator e)
{
	if constexpr(sizeof(*b)%sizeof(char_type)||sizeof(*e)%sizeof(char_type))
		if((e-b)*sizeof(*b)%sizeof(char_type))
			throw std::runtime_error("incorrect write size");
}

template<Signed_integer offset_type,typename T,typename char_type, Integral integr>
inline constexpr auto seek_precondition(integr i)
{
	std::make_unsigned_t<offset_type> constexpr offset_max(std::numeric_limits<offset_type>::max());
	i = i * static_cast<std::ptrdiff_t>(sizeof(T))/static_cast<std::ptrdiff_t>(sizeof(char_type));

	if constexpr(offset_max<static_cast<std::make_unsigned_t<integr>>(std::numeric_limits<integr>::max()))
	{
		if(offset_max<static_cast<std::make_unsigned_t<integr>>(i))
			throw std::runtime_error("index is not in range for seek type");
	}
	return static_cast<offset_type>(i);
}
}