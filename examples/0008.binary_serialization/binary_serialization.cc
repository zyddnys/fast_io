#include<iostream>
#include"../../include/fast_io.h"
#include<string>
#include<array>
#include<vector>
#include<deque>

int main()
{
	{
	fast_io::obuf out("binary_serialization.txt");
	std::vector<std::vector<std::array<std::size_t,3>>> arr{std::vector<std::array<std::size_t,3>>{std::array<std::size_t,3>{12142141,2,3},std::array<std::size_t,3>{4,5,6}}};
	write(out,arr);
	std::array<std::string,1> ar{"abcdef\n dsjfgoisdjgoisdjgdiso"};
	write(out,ar);
	}
	{
	fast_io::ibuf_mutex in("binary_serialization.txt");
	std::deque<std::vector<std::array<std::size_t,3>>> arr;
	std::string str;
	read(in,arr,str);
	for(auto const& e : arr)
		for(auto const& e1:e)
			for(auto const & e2 : e1)
				println(fast_io::out,e2);

	println(fast_io::out,str);
	}
}