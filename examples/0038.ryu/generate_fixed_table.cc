#include"../../include/fast_io.h"

inline constexpr auto pow10_bits_for_index(std::uint32_t idx)
{
	return (idx<<4)+128;
}

inline constexpr std::uint16_t length_for_index(std::uint64_t idx)
{
// [log_10(2^i)] = ((16 * i) * 78913L) >> 18 <-- floor
// +1 for ceil, +16 for mantissa, +8 to round up when dividing by 9
	return ((((16 * idx) * 1292913986) >> 32) + 25) / 9;
}

inline constexpr std::size_t length_for_index2(std::size_t idx)
{
	return ((120*idx)>>6)+3;
}


inline constexpr auto generate_fixed_offset_table()
{
	constexpr std::size_t table_size(64);
	std::array<std::uint16_t,table_size> offset{};
	std::uint16_t sum(0);
	for(std::size_t i(0);i!=table_size;++i)
	{
		offset[i]=sum;
		sum+=length_for_index(i);
	}
	return offset;
}

template<typename T>
inline constexpr auto fixed_sum_split(T const& offset)
{
	std::size_t sum(0);
	for(auto const & e : offset)
		sum+=e;
	return sum;
}

constexpr auto fixed_table(generate_fixed_offset_table());

inline constexpr auto generate_pow10_split()
{
	std::array<std::array<std::uint64_t,3>,fixed_sum_split(fixed_table)> split{};
	fast_io::uint256_t tpslb(1000000000);
	tpslb<<=136;
	std::size_t tot(0);
	for(std::size_t j(0);j!=64;++j)
	{
		std::uint32_t pow10bits(pow10_bits_for_index(j));
		fast_io::uint256_t vo(1);
		vo<<=pow10bits;
		std::size_t len(length_for_index(j));
		for(std::size_t i(0);i!=len;++i)
		{
			fast_io::uint256_t tenpowi(9*i);
			fast_io::uint256_t v(vo);
			v/=tenpowi;
			++v;
			v%=tpslb;
			for(std::size_t k(0);k!=3;++k)
			{
				fast_io::uint256_t bits(v);
				bits>>=64*k;
				split[tot][k]=static_cast<std::uint64_t>(bits);
			}
			++tot;
		}
	}
	return split;
}

auto split(generate_pow10_split());

int main()
{

	for(auto const & e : split)
		fprint(fast_io::out,"% % %\n",e[0],e[1],e[2]);
}
