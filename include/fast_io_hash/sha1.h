#pragma once

namespace fast_io
{

namespace details::sha1
{
namespace
{
//from https://github.com/vog/sha1/blob/master/sha1.cpp
inline constexpr std::uint32_t blk(std::array<std::uint32_t,16> const &block,std::size_t const i)
{
	return std::rotl(block[(i+13)&15] ^ block[(i+8)&15] ^ block[(i+2)&15] ^ block[i], 1);
}

inline constexpr void R0(std::array<std::uint32_t,16> const &block, std::uint32_t const v, std::uint32_t &w, std::uint32_t const x, std::uint32_t const y, std::uint32_t &z, const size_t i)
{
	z += ((w&(x^y))^y) + block[i] + 0x5a827999 + std::rotl(v, 5);
	w = std::rotl(w, 30);
}


inline constexpr void R1(std::array<std::uint32_t,16> &block, std::uint32_t const v, std::uint32_t &w, std::uint32_t const x, std::uint32_t const y, std::uint32_t &z, const size_t i)
{
	block[i] = blk(block, i);
	z += ((w&(x^y))^y) + block[i] + 0x5a827999 + std::rotl(v, 5);
	w = std::rotl(w, 30);
}


inline constexpr void R2(std::array<std::uint32_t,16> &block, std::uint32_t const v, std::uint32_t &w, std::uint32_t const x, std::uint32_t const y, std::uint32_t &z, const size_t i)
{
	block[i] = blk(block, i);
	z += (w^x^y) + block[i] + 0x6ed9eba1 + std::rotl(v, 5);
	w = std::rotl(w, 30);
}


inline constexpr void R3(std::array<std::uint32_t,16> &block, std::uint32_t const v, std::uint32_t &w, std::uint32_t const x, std::uint32_t const y, std::uint32_t &z, const size_t i)
{
	block[i] = blk(block, i);
	z += (((w|x)&y)|(w&x)) + block[i] + 0x8f1bbcdc + std::rotl(v, 5);
	w = std::rotl(w, 30);
}


inline constexpr void R4(std::array<std::uint32_t,16> &block, std::uint32_t const v, std::uint32_t &w, std::uint32_t const x, std::uint32_t const y, std::uint32_t &z, const size_t i)
{
	block[i] = blk(block, i);
	z += (w^x^y) + block[i] + 0xca62c1d6 + std::rotl(v, 5);
	w = std::rotl(w, 30);
}


/*
 * Hash a single 512-bit block. This is the core of the algorithm.
 */

inline constexpr void transform(std::array<std::uint32_t,5>& digest, std::array<std::uint32_t,16> &block)
{
	/* Copy digest[] to working vars */
	std::uint32_t a(digest.front());
	std::uint32_t b(digest[1]);
	std::uint32_t c(digest[2]);
	std::uint32_t d(digest[3]);
	std::uint32_t e(digest.back());

	/* 4 rounds of 20 operations each. Loop unrolled. */
	R0(block, a, b, c, d, e,  0);
	R0(block, e, a, b, c, d,  1);
	R0(block, d, e, a, b, c,  2);
	R0(block, c, d, e, a, b,  3);
	R0(block, b, c, d, e, a,  4);
	R0(block, a, b, c, d, e,  5);
	R0(block, e, a, b, c, d,  6);
	R0(block, d, e, a, b, c,  7);
	R0(block, c, d, e, a, b,  8);
	R0(block, b, c, d, e, a,  9);
	R0(block, a, b, c, d, e, 10);
	R0(block, e, a, b, c, d, 11);
	R0(block, d, e, a, b, c, 12);
	R0(block, c, d, e, a, b, 13);
	R0(block, b, c, d, e, a, 14);
	R0(block, a, b, c, d, e, 15);
	R1(block, e, a, b, c, d,  0);
	R1(block, d, e, a, b, c,  1);
	R1(block, c, d, e, a, b,  2);
	R1(block, b, c, d, e, a,  3);
	R2(block, a, b, c, d, e,  4);
	R2(block, e, a, b, c, d,  5);
	R2(block, d, e, a, b, c,  6);
	R2(block, c, d, e, a, b,  7);
	R2(block, b, c, d, e, a,  8);
	R2(block, a, b, c, d, e,  9);
	R2(block, e, a, b, c, d, 10);
	R2(block, d, e, a, b, c, 11);
	R2(block, c, d, e, a, b, 12);
	R2(block, b, c, d, e, a, 13);
	R2(block, a, b, c, d, e, 14);
	R2(block, e, a, b, c, d, 15);
	R2(block, d, e, a, b, c,  0);
	R2(block, c, d, e, a, b,  1);
	R2(block, b, c, d, e, a,  2);
	R2(block, a, b, c, d, e,  3);
	R2(block, e, a, b, c, d,  4);
	R2(block, d, e, a, b, c,  5);
	R2(block, c, d, e, a, b,  6);
	R2(block, b, c, d, e, a,  7);
	R3(block, a, b, c, d, e,  8);
	R3(block, e, a, b, c, d,  9);
	R3(block, d, e, a, b, c, 10);
	R3(block, c, d, e, a, b, 11);
	R3(block, b, c, d, e, a, 12);
	R3(block, a, b, c, d, e, 13);
	R3(block, e, a, b, c, d, 14);
	R3(block, d, e, a, b, c, 15);
	R3(block, c, d, e, a, b,  0);
	R3(block, b, c, d, e, a,  1);
	R3(block, a, b, c, d, e,  2);
	R3(block, e, a, b, c, d,  3);
	R3(block, d, e, a, b, c,  4);
	R3(block, c, d, e, a, b,  5);
	R3(block, b, c, d, e, a,  6);
	R3(block, a, b, c, d, e,  7);
	R3(block, e, a, b, c, d,  8);
	R3(block, d, e, a, b, c,  9);
	R3(block, c, d, e, a, b, 10);
	R3(block, b, c, d, e, a, 11);
	R4(block, a, b, c, d, e, 12);
	R4(block, e, a, b, c, d, 13);
	R4(block, d, e, a, b, c, 14);
	R4(block, c, d, e, a, b, 15);
	R4(block, b, c, d, e, a,  0);
	R4(block, a, b, c, d, e,  1);
	R4(block, e, a, b, c, d,  2);
	R4(block, d, e, a, b, c,  3);
	R4(block, c, d, e, a, b,  4);
	R4(block, b, c, d, e, a,  5);
	R4(block, a, b, c, d, e,  6);
	R4(block, e, a, b, c, d,  7);
	R4(block, d, e, a, b, c,  8);
	R4(block, c, d, e, a, b,  9);
	R4(block, b, c, d, e, a, 10);
	R4(block, a, b, c, d, e, 11);
	R4(block, e, a, b, c, d, 12);
	R4(block, d, e, a, b, c, 13);
	R4(block, c, d, e, a, b, 14);
	R4(block, b, c, d, e, a, 15);

	/* Add the working vars back into digest[] */
	digest[0] += a;
	digest[1] += b;
	digest[2] += c;
	digest[3] += d;
	digest[4] += e;
}
}
}
inline namespace
{

struct sha1
{
	using char_type = char;
	std::array<std::uint32_t,5> digest{0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0};
	std::array<std::uint32_t,16> block{};
	char* it=static_cast<char*>(static_cast<void*>(block.data())),*ed=it+64;
	std::uint64_t transforms{};
};

inline constexpr void clear(sha1& sh)
{
	sh=sha1();
}

inline constexpr void put(sha1& sh,char ch)
{
	if(sh.it==sh.ed) [[unlikely]]
	{
		if constexpr(std::endian::native==std::endian::little)
			for(auto& e : sh.block)
				e=details::big_endian(e);
		details::sha1::transform(sh.digest,sh.block);
		++sh.transforms;
		sh.it=sh.ed-64;
	}
	*sh.it=ch;
	++sh.it;
}

inline constexpr auto oreserve(sha1& sh,std::size_t size) -> decltype(sh.it)
{
	if(sh.ed<sh.it+size)
		return nullptr;
	return sh.it+=size;
}

inline constexpr void orelease(sha1& sh,std::size_t size)
{
	sh.it-=size;
}

template<std::contiguous_iterator Iter>
inline constexpr void writes(sha1& sh,Iter cbegin,Iter cend)
{
	auto b(static_cast<char const*>(static_cast<void const*>(std::to_address(cbegin))));
	auto e(static_cast<char const*>(static_cast<void const*>(std::to_address(cend))));
	for(std::size_t n{};(n=static_cast<std::size_t>(sh.ed-sh.it))<static_cast<std::size_t>(e-b);b+=n)
	{
		sh.it=std::copy_n(b,n,sh.it)-64;
		if constexpr(std::endian::native==std::endian::little)
			for(auto& e : sh.block)
				e=details::big_endian(e);
		details::sha1::transform(sh.digest,sh.block);
		++sh.transforms;
	}
	sh.it=std::copy(b,e,sh.it);
}

[[deprecated("sha1 is no longer a secure algorithm, see wikipedia. The SHAppening: https://en.wikipedia.org/wiki/SHA-1#The_SHAppening")]] 
inline constexpr void flush(sha1& sh)
{
	put(sh,0x80);
	std::uint64_t const total_count((sh.transforms*64+(63-(sh.ed-sh.it)))<<3);
	std::fill(sh.it,sh.ed,0);
	if constexpr(std::endian::native==std::endian::little)
		for(auto& e : sh.block)
			e=details::big_endian(e);
	sh.block[sh.block.size()-2]=static_cast<std::uint32_t>(total_count>>32);
	sh.block.back()=static_cast<std::uint32_t>(total_count);
	details::sha1::transform(sh.digest,sh.block);

}

template<std::contiguous_iterator Iter>
inline constexpr Iter reads(sha1& sh,Iter begin,Iter end)
{
	auto b(static_cast<char*>(static_cast<void*>(std::to_address(begin))));
	auto e(static_cast<char*>(static_cast<void*>(std::to_address(end))));
	std::size_t n(64);
	if(static_cast<std::size_t>(e-b)<n)
		n=static_cast<std::size_t>(e-b);
	std::copy_n(static_cast<char const*>(static_cast<void const*>(sh.digest.data())),n,b);
	return b+n/sizeof(*b);
}

template<buffer_output_stream output>
inline constexpr void print_define(output& out,sha1 const& sh)
{
	for(auto const & e : sh.digest)
		print(out,width<8,false,'0'>(hex(e)));
}

}

}