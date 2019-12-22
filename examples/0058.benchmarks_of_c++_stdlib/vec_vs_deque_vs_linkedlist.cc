#include<vector>
#include<forward_list>
#include<deque>
#include"timer.h"

int main()
try
{
	std::vector<std::size_t> result(3);
	constexpr std::size_t N(10000000);
	constexpr std::size_t M(100);
{
	cqw::timer t("forward_list total");
	std::forward_list<std::size_t> fwd;		//forward_list is singly linked list in C++
	{
	cqw::timer t1("forward_list emplace_front");	
	for(std::size_t i(0);i!=N;++i)
		fwd.emplace_front(i);
	}
	{
	cqw::timer t("traversal");
	std::size_t sum(0);
	for(std::size_t i(0);i!=M;++i)
		for(auto const & e : fwd)
			sum^=e;
	result[0]=sum;//prevent compiler optimization for looping
	}
	{
	cqw::timer t("destroy");
	fwd.clear();
	}
}
{
	cqw::timer t("vector total");
	std::vector<std::size_t> fwd;		//forward_list is singly linked list in C++
	{
	cqw::timer t1("vector emplace_back");	
	for(std::size_t i(0);i!=N;++i)
		fwd.emplace_back(i);
	}
	{
	cqw::timer t("traversal");
	std::size_t sum(0);
	for(std::size_t i(0);i!=M;++i)
		for(auto const & e : fwd)
			sum^=e;
	result[1]=sum;//prevent compiler optimization for looping
	}
	{
	cqw::timer t("destroy");
	fwd.clear();
	}
}

{
	cqw::timer t("deque total");
	std::deque<std::size_t> fwd;		//forward_list is singly linked list in C++
	{
	cqw::timer t1("deque emplace_back");	
	for(std::size_t i(0);i!=N;++i)
		fwd.emplace_back(i);
	}
	{
	cqw::timer t("traversal");
	std::size_t sum(0);
	for(std::size_t i(0);i!=M;++i)
		for(auto const & e : fwd)
			sum^=e;
	result[2]=sum;//prevent compiler optimization for looping
	}
	{
	cqw::timer t("destroy");
	fwd.clear();
	}
}
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
}

/*
D:\hg\fast_io\examples\0058.benchmarks_of_c++_stdlib>g++ -o vec_vs_deque_vs_linkedlist vec_vs_deque_vs_linkedlist.cc -Ofast -std=c++2a -s

D:\hg\fast_io\examples\0058.benchmarks_of_c++_stdlib>vec_vs_deque_vs_linkedlist
forward_list emplace_front:     0.35101000000000004s
traversal:      10.515s
destroy:        0.30000000000000004s
forward_list total:     11.16601s
vector emplace_back:    0.047s
traversal:      0.488991s
destroy:        0s
vector total:   0.539989s
deque emplace_back:     0.037000000000000005s
traversal:      1.1480110000000001s
destroy:        0.024s
deque total:    1.209011s

*/