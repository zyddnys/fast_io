#pragma once

#include <cstdint>
#include "../concept.h"

namespace fast_io::crypto
{

namespace details::speck
{

inline std::uint64_t ROR(std::uint64_t x, std::uint64_t r) {return ((x >> r) | (x << (64 - r)));}
inline std::uint64_t ROL(std::uint64_t x,std::uint64_t  r) {return (x << r) | (x >> (64 - r));}
inline std::uint64_t R(std::uint64_t& x, std::uint64_t& y, std::uint64_t& k)
{
    x = ROR(x, 8);
    x += y;
    x ^= k;
    y = ROL(y, 3);
    y ^= x;
}
}
/*
template<output_stream output, std::size_t key_size = 128, std::size_t round = 32>
class basic_ospeck
{
    std::array<uint64_t, key_size / 64> key;
    output& out;
public:
    using native_interface_t = output;
	using char_type = typename native_interface_t::char_type;


    basic_ospeck(std::array<uint64_t, key_size / 64> const& init_key, output& o) : key(init_key), out(o)
    {

    }

    template<typename ContiguousIterator>
	ContiguousIterator write(ContiguousIterator b, ContiguousIterator e)
	{
		write_precondition<char_type>(b, e);
        auto pb(static_cast<char_type const*>(static_cast<void const*>(std::addressof(*b))));
		auto last(pb);
		auto pi(pb), pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));
        if ((e-b)%128==0)
        {
            uint64_t y = pt[0], x = pt[1], b = key[0], a = key[1];

            details::speck::R(x, y, b);
            for (std::size_t i = 0; i < round - 1; ++i) {
                details::speck::R(a, b, i);
                details::speck::R(x, y, b);
            }

            ct[0] = y;
            ct[1] = x;
        }
        else
        {
            /* code */
        }
        
	}

    void flush()
	{
		// no need fsync. OS can deal with it
//		if(::fsync(fd)==-1)
//			throw std::system_error(errno,std::generic_category());
	}
};
*/
template<output_stream output, std::size_t key_size = 128, std::size_t round = 32>
class basic_ospeck
{
    std::array<uint64_t, key_size / 64> key;
    output& out;
public:
    using native_interface_t = output;
	using char_type = typename native_interface_t::char_type;


    basic_ospeck(std::array<uint64_t, key_size / 64> const& init_key, output& o) : key(init_key), out(o)
    {

    }

    template<typename ContiguousIterator>
	ContiguousIterator write(ContiguousIterator b, ContiguousIterator e)
	{
		write_precondition<char_type>(b, e);
        auto pb(static_cast<char_type const*>(static_cast<void const*>(std::addressof(*b))));
		auto last(pb);
		auto pi(pb), pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));
        if ((e-b)%128==0)
        {
            uint64_t y = pt[0], x = pt[1], b = key[0], a = key[1];

            details::speck::R(x, y, b);
            for (std::size_t i = 0; i < round - 1; ++i) {
                details::speck::R(a, b, i);
                details::speck::R(x, y, b);
            }

            ct[0] = y;
            ct[1] = x;
        }
        else
        {
            /* code */
        }
        
	}

    void flush()
	{
		// no need fsync. OS can deal with it
//		if(::fsync(fd)==-1)
//			throw std::system_error(errno,std::generic_category());
	}
};

}
