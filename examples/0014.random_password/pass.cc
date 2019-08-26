#include"../../include/fast_io.h"
#include<vector>
#include<cctype>
#include<random>

int main()
{
	std::random_device rdv;
	std::vector<decltype(rdv)::result_type> seeds;
	for(std::size_t i(0);i!=1000;++i)
		seeds.emplace_back(rdv());
	std::seed_seq sq(seeds.cbegin(),seeds.cend());
	std::mt19937_64 eng(sq);
	std::uniform_int_distribution<char> ud(0,61);
	std::uniform_int_distribution<std::size_t> rlen(8,20);
	fast_io::obuf ob("pass.txt");
	for(std::size_t i(0);i!=1000;++i)
	{
		for(std::size_t j(0),s(rlen(eng));j!=s;++j)
		{
			auto ch(ud(eng));
			if(ch<10)
				ob.put(ch+'0');
			else if(ch<36)
				ob.put(ch-10+'a');
			else
				ob.put(ch-36+'A');
		}
		ob.put('\n');
	}
}