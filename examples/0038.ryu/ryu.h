#pragma once
#include"../../include/fast_io.h"
#ifdef _M_X64
#include <intrin0.h> // for _umul128() and __shiftright128()
#endif // _M_X64

namespace fast_io
{
namespace ryu_details
{
namespace
{
inline constexpr std::uint32_t pow5_bits(std::uint32_t e)
{
	return static_cast<uint32_t> (((e * 163391164108059ull) >> 46) + 1);
}

struct u128
{
	uint64_t a,b;
};

struct u256
{
//	uint64_t a,b,c,d;
	uint128_t a,b;
};

inline constexpr u256 mul_128_256_shift(u128 const& a, u128 const& b, uint32_t shift,uint32_t corr)
{
	uint128_t const aa(static_cast<uint128_t>(a.a));
	uint128_t const b00(aa * b.a); // 0
	uint128_t const b01(aa * b.b); // 64
	uint128_t const b02(aa * b.c); // 128
	uint128_t const b03(aa * b.d); // 196
	uint128_t const ab(static_cast<uint128_t>(a.b));
	uint128_t const b10(ab * b.a); // 0
	uint128_t const b11(ab * b.b); // 64
	uint128_t const b12(ab * b.c); // 128
	uint128_t const b13(ab * b.d); // 196

	uint128_t const s0(b00);       // 0   x
	uint128_t const s1(b01 + b10); // 64  x
	auto const c1(s1 < b01);  // 196 x
	uint128_t const s2(b02 + b11); // 128 x
	auto const c2(s2 < b02);  // 256 x
	uint128_t const s3(b03 + b12); // 196 x
	auto const c3(s3 < b03);  // 324 x

	uint128_t const p0(s0 + (s1 << 64));                                // 0
	uint128_t const d0(p0 < b00);                                       // 128
	uint128_t const q1(s2 + (s1 >> 64) + (s3 << 64));                   // 128
	auto const d1(q1 < s2);                                        // 256
	uint128_t const p1(q1 + (c1 << 64) + d0);                           // 128
	auto const d2(p1 < q1);                                        // 256
	uint128_t const p2(b13 + (s3 >> 64) + c2 + (c3 << 64) + d1 + d2);   // 256

	if (shift < 128)
	{
/*		uint128_t const r0(corr + ((p0 >> shift) | (p1 << (128 - shift))));
		uint128_t const r1(((p1 >> shift) | (p2 << (128 - shift))) + (r0 < corr));
		return {static_cast<std::uint64_t>(r0),static_cast<std::uint64_t>(r0 >> 64),static_cast<std::uint64_t>(r1),static_cast<std::uint64_t>(r1>>64)};*/
		return {corr + ((p0 >> shift) | (p1 << (128 - shift))),((p1 >> shift) | (p2 << (128 - shift))) + (r0 < corr)};
	}
	else if (shift == 128)
	{
/*		uint128_t const r0(corr + p1);
		uint128_t const r1(p2 + (r0 < corr));
		return {static_cast<std::uint64_t>(r0),static_cast<std::uint64_t>(r0 >> 64),static_cast<std::uint64_t>(r1),static_cast<std::uint64_t>(r1 >> 64)}*/
		return {corr + p1,p2 + (r0 < corr)};
	}
	return {corr + ((p1 >> (shift - 128)) | (p2 << (256 - shift))),(p2 >> (shift - 128)) + (r0 < corr)};
/*
	uint128_t const r0(corr + ((p1 >> (shift - 128)) | (p2 << (256 - shift))));
	uint128_t const r1((p2 >> (shift - 128)) + (r0 < corr));
    result[0] = (uint64_t) r0;
    result[1] = (uint64_t) (r0 >> 64);
    result[2] = (uint64_t) r1;
    result[3] = (uint64_t) (r1 >> 64);*/
}



template<typename T>
requires std::unsigned_integral<T>
inline constexpr T log10Pow5(T e)
{
	return (e*732923)>>20;
}

template<typename T>
requires std::unsigned_integral<T>
inline constexpr std::size_t decimal_length(T u)
{
	if constexpr(4<sizeof(T))
	{
/*		if(9999999999999999999ull<u)
			return 20;
		if(999999999999999999ull<u)
			return 19;
		if(99999999999999999ull<u)
			return 18;*/
		if(9999999999999999ull<u)
			return 17;
		if(999999999999999ull<u)
			return 16;
		if(99999999999999ull<u)
			return 15;
		if(9999999999999ull<u)
			return 14;
		if(999999999999ull<u)
			return 13;
		if(99999999999ull<u)
			return 12;
		if(9999999999ull<u)
			return 11;
	}
	if constexpr(2<sizeof(T))
	{
	if(999999999<u)
		return 10;
	if(99999999<u)
		return 9;
	if(9999999<u)
		return 8;
	if(999999<u)
		return 7;
	if(99999<u)
		return 6;
	}
	if constexpr(1<sizeof(T))
	{
	if(9999<u)
		return 5;
	if(999<u)
		return 4;
	}
	if(99<u)
		return 3;
	if(9<u)
		return 2;
	return 1;
}
}
}
}
