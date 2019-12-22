#include<vector>
#ifdef __clang__
#include"timer2.h"
#else
#include"timer.h"
#endif

int main()
{
	constexpr std::size_t N(100000000);
{
	cqw::timer tm("vector reduce dependency");
	std::vector<std::size_t> vec;
	for(vec.reserve(N);vec.size()!=vec.capacity();vec.emplace_back(vec.size()));
}
{
	cqw::timer tm("vector reduce dependency");
	std::vector<std::size_t> vec;
	for(vec.reserve(N);vec.size()!=vec.capacity();vec.emplace_back(vec.size()));
}
{
	cqw::timer tm("vector");
	std::vector<std::size_t> vec;
	vec.reserve(N);
	for(std::size_t i(0);i!=N;++i)
		vec.emplace_back(i);
}
{
	cqw::timer tm("vector reduce dependency");
	std::vector<std::size_t> vec;
	for(vec.reserve(N);vec.size()!=vec.capacity();vec.emplace_back(vec.size()));
}
/*{
	cqw::timer tm("vector reduce unchecked");
	std::vector<std::size_t> vec;
	for(vec.reserve(N);vec.size()!=vec.capacity();vec.emplace_back_unchecked(vec.size()));
}
{
	cqw::timer tm("vector emplace_back_unchecked");
	std::vector<std::size_t> vec;
	vec.reserve(N);
	for(std::size_t i(0);i!=N;++i)
		vec.emplace_back_unchecked(i);
}*/
{
	cqw::timer tm("vector death");
	std::vector<std::size_t> vec;
	vec.reserve(N);
	for(std::size_t i{};vec.size()!=vec.capacity();vec.emplace_back(i++));
}
{
	cqw::timer tm("vector death1");
	std::vector<std::size_t> vec;
	vec.reserve(N);
	for(std::size_t i{};vec.size()!=vec.capacity();++i)
     	   vec.emplace_back(i);
}

}
