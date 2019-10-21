#pragma once

/*
ryu source code on github is OBJECTIVELY STUPID SHIT. C programmers are all garbage
*/

namespace fast_io
{
namespace ryu_details
{

template<typename T>
requires std::floating_point<T>	
struct floating_traits
{
};

template<>	
struct floating_traits<double>
{
	static inline constexpr std::size_t mantissa_bits = 52;
	static inline constexpr std::size_t exponent_bits = 11;
	static inline constexpr std::size_t bias = 1023;
	static inline constexpr std::size_t pow5_inv_bitcount= 122;
	static inline constexpr std::size_t pow5_bitcount= 121;
};

template<typename mantissaType,typename exponentType>
struct unrep
{
//	using floating_type = floating;
	using mantissa_type = mantissaType;
	using exponent_type = exponentType;
//	inline static constexpr std::size_t base = Base;
	mantissa_type m=0;
	exponent_type e=0;
};

// I am not a native english speaker. bin/oct/dec/hex are OBJECTIVELY stupid for me.
/*
emplate<std::floating_point floating,mantissaType,exponentType>
inline constexpr auto base2_to_base10(floating_unsigned_representation<floating,mantissaType,exponentType,2> ieee)
{
	floating_unsigned_representation<floating,mantissaType,exponentType,10> rep;
	if(!ieee.exponent)
	{
		rep.mantissa = ieee.mantissa;
		rep.exponent = -1-floating_traits<floating>::bias-floating_traits<floating>::bits;
	}
	else
	{
		rep.exponent = ieee.exponent-floating_traits<floating>::bias-floating_traits<floating>::bits-2;
		rep.mantissa = static_cast<mantissaType>(1)<<floating_traits<floating>::bits|ieee.mantissa;
	}
	bool const even(rep.mantissa&1);
	mantissaType const mv(rep.mantissa<<2);
	exponentType const mmShift(ieee.mantissa||ieee.exponent<2);
	
}*/

template<typename T,std::floating_to_point F>
inline constexpr T to_bits(F const& f)
{
	union
	{
		F d;
		T u;
	}k{f};
	return k.u;
/*	bool sign(k.u>>63);
	std::uint64_t exponent((k.u<<static_cast<std::uint64_t>(1))>>static_cast<std::uint64_t>(53));
	std::uint64_t constexpr fractionfield((static_cast<std::uint64_t>(1)<<static_cast<std::uint64_t>(52))-static_cast<std::uint64_t>(1));
	std::uint64_t fraction(k.u&fractionfield);
	return */
}

template<standard_output_stream output,typename mantissaType>
inline constexpr void easy_case(output& out,bool sign, mantissaType const& mantissa)
{
	if (mantissa)
	{
		print(out,"nan");
		return;
	}
	if (sign)
		out.put('-');
	print(out,"Inf");
}

template<typename mantissaType,typename exponentType>
inline constexpr unrep<mantissaType,exponentType> init_rep(mantissaType const& mantissa,exponentType const& exponent)
{
	if(!exponent)
		return {mantissa,-1-floating_traits<floating>::bias-floating_traits<floating>::bits}
	return {static_cast<mantissaType>(1)<<floating_traits<floating>::bits|mantissa,exponent-floating_traits<floating>::bias-floating_traits<floating>::bits-2};
}

template<standard_output_stream output,typename T,typename E,std::floating_point F>
inline constexpr void output_fixed(output& out, F d,std::size_t precision)
{
	auto const bits(to_bits<T>(d));
	// Decode bits into sign, mantissa, and exponent.
	bool const sign((bits >> (floating_traits<F>::mantissa_bits + floating_traits<F>::exponent_bits)) & 1u);
	T const mantissa(bits & ((static_cast<T>(1u) << floating_traits<F>::mantissa_bits) - 1u));
	E const exponent(static_cast<E>(((bits >> floating_traits<F>::mantissa_bits) & ((static_cast<E>(1u) << floating_traits<F>::exponent_bits) - 1u)));

	// Case distinction; exit early for the easy cases.
	if (exponent == ((1u << floating_traits<F>::exponent_bits) - 1u))
	{
		easy_case(out,sign,mantissa);
		return;
	}
	if (!exponent&& !mantissa)
	{
		if(sign)
			out.put('-');
		if(precision)
		{
			out.put('.');
			fill_nc(out,precision,0);
		}
		return;
	}

	auto const r2(init_rep(mantissa,static_cast<std::make_signed_t>(exponent)));
	std::size_t index(0);
	if (sign)
	{
		out.put('-');
		++index;
	}
	bool nonzero(false);
	if (-52<=e2)
	{
	const uint32_t idx = e2 < 0 ? 0 : indexForExponent((uint32_t) );
	const uint32_t p10bits = pow10BitsForIndex(idx);
	const int32_t len = (int32_t) lengthForIndex(idx);

	for (int32_t i = len - 1; i >= 0; --i) {
	const uint32_t j = p10bits - e2;
	// Temporary: j is usually around 128, and by shifting a bit, we push it to 128 or above, which is
	// a slightly faster code path in mulShift_mod1e9. Instead, we can just increase the multipliers.
	const uint32_t digits = mulShift_mod1e9(m2 << 8, POW10_SPLIT[POW10_OFFSET[idx] + i], (int32_t) (j + 8));
	if (nonzero) {
	append_nine_digits(digits, result + index);
	index += 9;
	} else if (digits != 0) {
	const uint32_t olength = decimalLength9(digits);
	append_n_digits(olength, digits, result + index);
	index += olength;
	nonzero = true;
	}
	}
	}
	if (!nonzero) {
	result[index++] = '0';
	}
	if (precision > 0) {
	result[index++] = '.';
	}
	if (e2 < 0) {
	const int32_t idx = -e2 / 16;
	const uint32_t blocks = precision / 9 + 1;
	// 0 = don't round up; 1 = round up unconditionally; 2 = round up if odd.
	int roundUp = 0;
	uint32_t i = 0;
	if (blocks <= MIN_BLOCK_2[idx]) {
	i = blocks;
	memset(result + index, '0', precision);
	index += precision;
	} else if (i < MIN_BLOCK_2[idx]) {
	i = MIN_BLOCK_2[idx];
	memset(result + index, '0', 9 * i);
	index += 9 * i;
	}
	for (; i < blocks; ++i) {
	const int32_t j = ADDITIONAL_BITS_2 + (-e2 - 16 * idx);
	const uint32_t p = POW10_OFFSET_2[idx] + i - MIN_BLOCK_2[idx];
	if (p >= POW10_OFFSET_2[idx + 1]) {
	// If the remaining digits are all 0, then we might as well use memset.
	// No rounding required in this case.
	const uint32_t fill = precision - 9 * i;
	memset(result + index, '0', fill);
	index += fill;
	break;
	}
	// Temporary: j is usually around 128, and by shifting a bit, we push it to 128 or above, which is
	// a slightly faster code path in mulShift_mod1e9. Instead, we can just increase the multipliers.
	uint32_t digits = mulShift_mod1e9(m2 << 8, POW10_SPLIT_2[p], j + 8);
	if (i < blocks - 1) {
	append_nine_digits(digits, result + index);
	index += 9;
	} else {
	const uint32_t maximum = precision - 9 * i;
	uint32_t lastDigit = 0;
	for (uint32_t k = 0; k < 9 - maximum; ++k) {
	lastDigit = digits % 10;
	digits /= 10;
	}
	if (lastDigit != 5) {
	roundUp = lastDigit > 5;
	} else {
	// Is m * 10^(additionalDigits + 1) / 2^(-e2) integer?
	const int32_t requiredTwos = -e2 - (int32_t) precision - 1;
	const bool trailingZeros = requiredTwos <= 0
	|| (requiredTwos < 60 && multipleOfPowerOf2(m2, (uint32_t) requiredTwos));
	roundUp = trailingZeros ? 2 : 1;

	}
	if (maximum > 0) {
	append_c_digits(maximum, digits, result + index);
	index += maximum;
	}
	break;
	}
	}
	if (roundUp != 0) {
	int roundIndex = index;
	int dotIndex = 0; // '.' can't be located at index 0
	while (true) {
	--roundIndex;
	char c;
	if (roundIndex == -1 || (c = result[roundIndex], c == '-')) {
	result[roundIndex + 1] = '1';
	if (dotIndex > 0) {
	result[dotIndex] = '0';
	result[dotIndex + 1] = '.';
	}
	result[index++] = '0';
	break;
	}
	if (c == '.') {
	dotIndex = roundIndex;
	continue;
	} else if (c == '9') {
	result[roundIndex] = '0';
	roundUp = 1;
	continue;
	} else {
	if (roundUp == 2 && c % 2 == 0) {
	break;
	}
	result[roundIndex] = c + 1;
	break;
	}
	}
	}
	} else {
	memset(result + index, '0', precision);
	index += precision;
	}
	return index;
}

/*
floating_decimal_128 generic_binary_to_decimal(
    const uint128_t bits, const uint32_t mantissaBits, const uint32_t exponentBits, const bool explicitLeadingBit) {

  const uint32_t bias = (1u << (exponentBits - 1)) - 1;
  const bool ieeeSign = ((bits >> (mantissaBits + exponentBits)) & 1) != 0;
  const uint128_t ieeeMantissa = bits & ((ONE << mantissaBits) - 1);
  const uint32_t ieeeExponent = (uint32_t) ((bits >> mantissaBits) & ((ONE << exponentBits) - 1u));

  if (ieeeExponent == 0 && ieeeMantissa == 0) {
    struct floating_decimal_128 fd;
    fd.mantissa = 0;
    fd.exponent = 0;
    fd.sign = ieeeSign;
    return fd;
  }
  if (ieeeExponent == ((1u << exponentBits) - 1u)) {
    struct floating_decimal_128 fd;
    fd.mantissa = explicitLeadingBit ? ieeeMantissa & ((ONE << (mantissaBits - 1)) - 1) : ieeeMantissa;
    fd.exponent = FD128_EXCEPTIONAL_EXPONENT;
    fd.sign = ieeeSign;
    return fd;
  }

  int32_t e2;
  uint128_t m2;
  // We subtract 2 in all cases so that the bounds computation has 2 additional bits.
  if (explicitLeadingBit) {
    // mantissaBits includes the explicit leading bit, so we need to correct for that here.
    if (ieeeExponent == 0) {
      e2 = 1 - bias - mantissaBits + 1 - 2;
    } else {
      e2 = ieeeExponent - bias - mantissaBits + 1 - 2;
    }
    m2 = ieeeMantissa;
  } else {
    if (ieeeExponent == 0) {
      e2 = 1 - bias - mantissaBits - 2;
      m2 = ieeeMantissa;
    } else {
      e2 = ieeeExponent - bias - mantissaBits - 2;
      m2 = (ONE << mantissaBits) | ieeeMantissa;
    }
  }
  const bool even = (m2 & 1) == 0;
  const bool acceptBounds = even;


  // Step 2: Determine the interval of legal decimal representations.
  const uint128_t mv = 4 * m2;
  // Implicit bool -> int conversion. True is 1, false is 0.
  const uint32_t mmShift = (ieeeMantissa != 0) || (ieeeExponent == 0);

  // Step 3: Convert to a decimal power base using 128-bit arithmetic.
  uint128_t vr, vp, vm;
  int32_t e10;
  bool vmIsTrailingZeros = false;
  bool vrIsTrailingZeros = false;
  if (e2 >= 0) {
    // I tried special-casing q == 0, but there was no effect on performance.
    // This expression is slightly faster than max(0, log10Pow2(e2) - 1).
    const uint32_t q = log10Pow2(e2) - (e2 > 3);
    e10 = q;
    const int32_t k = FLOAT_128_POW5_INV_BITCOUNT + pow5bits(q) - 1;
    const int32_t i = -e2 + q + k;
    uint64_t pow5[4];
    generic_computeInvPow5(q, pow5);
    vr = mulShift(4 * m2, pow5, i);
    vp = mulShift(4 * m2 + 2, pow5, i);
    vm = mulShift(4 * m2 - 1 - mmShift, pow5, i);

    // floor(log_5(2^128)) = 55, this is very conservative
    if (q <= 55) {
      // Only one of mp, mv, and mm can be a multiple of 5, if any.
      if (mv % 5 == 0) {
        vrIsTrailingZeros = multipleOfPowerOf5(mv, q - 1);
      } else if (acceptBounds) {
        // Same as min(e2 + (~mm & 1), pow5Factor(mm)) >= q
        // <=> e2 + (~mm & 1) >= q && pow5Factor(mm) >= q
        // <=> true && pow5Factor(mm) >= q, since e2 >= q.
        vmIsTrailingZeros = multipleOfPowerOf5(mv - 1 - mmShift, q);
      } else {
        // Same as min(e2 + 1, pow5Factor(mp)) >= q.
        vp -= multipleOfPowerOf5(mv + 2, q);
      }
    }
  } else {
    // This expression is slightly faster than max(0, log10Pow5(-e2) - 1).
    const uint32_t q = log10Pow5(-e2) - (-e2 > 1);
    e10 = q + e2;
    const int32_t i = -e2 - q;
    const int32_t k = pow5bits(i) - FLOAT_128_POW5_BITCOUNT;
    const int32_t j = q - k;
    uint64_t pow5[4];
    generic_computePow5(i, pow5);
    vr = mulShift(4 * m2, pow5, j);
    vp = mulShift(4 * m2 + 2, pow5, j);
    vm = mulShift(4 * m2 - 1 - mmShift, pow5, j);

    if (q <= 1) {
      // {vr,vp,vm} is trailing zeros if {mv,mp,mm} has at least q trailing 0 bits.
      // mv = 4 m2, so it always has at least two trailing 0 bits.
      vrIsTrailingZeros = true;
      if (acceptBounds) {
        // mm = mv - 1 - mmShift, so it has 1 trailing 0 bit iff mmShift == 1.
        vmIsTrailingZeros = mmShift == 1;
      } else {
        // mp = mv + 2, so it always has at least one trailing 0 bit.
        --vp;
      }
    } else if (q < 127) { // TODO(ulfjack): Use a tighter bound here.
      // We need to compute min(ntz(mv), pow5Factor(mv) - e2) >= q-1
      // <=> ntz(mv) >= q-1  &&  pow5Factor(mv) - e2 >= q-1
      // <=> ntz(mv) >= q-1    (e2 is negative and -e2 >= q)
      // <=> (mv & ((1 << (q-1)) - 1)) == 0
      // We also need to make sure that the left shift does not overflow.
      vrIsTrailingZeros = multipleOfPowerOf2(mv, q - 1);
    }
  }

  // Step 4: Find the shortest decimal representation in the interval of legal representations.
  uint32_t removed = 0;
  uint8_t lastRemovedDigit = 0;
  uint128_t output;

  while (vp / 10 > vm / 10) {
    vmIsTrailingZeros &= vm % 10 == 0;
    vrIsTrailingZeros &= lastRemovedDigit == 0;
    lastRemovedDigit = (uint8_t) (vr % 10);
    vr /= 10;
    vp /= 10;
    vm /= 10;
    ++removed;
  }
  if (vmIsTrailingZeros) {
    while (vm % 10 == 0) {
      vrIsTrailingZeros &= lastRemovedDigit == 0;
      lastRemovedDigit = (uint8_t) (vr % 10);
      vr /= 10;
      vp /= 10;
      vm /= 10;
      ++removed;
    }
  }
  if (vrIsTrailingZeros && (lastRemovedDigit == 5) && (vr % 2 == 0)) {
    // Round even if the exact numbers is .....50..0.
    lastRemovedDigit = 4;
  }
  // We need to take vr+1 if vr is outside bounds or we need to round up.
  output = vr +
      ((vr == vm && (!acceptBounds || !vmIsTrailingZeros)) || (lastRemovedDigit >= 5));
  const int32_t exp = e10 + removed;


  struct floating_decimal_128 fd;
  fd.mantissa = output;
  fd.exponent = exp;
  fd.sign = ieeeSign;
  return fd;
}*/

}
}