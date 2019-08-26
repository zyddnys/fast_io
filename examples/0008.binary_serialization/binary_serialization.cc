#include<iostream>
#include"../../include/fast_io.h"
#include<string>
#include<array>
#include<vector>
#include<deque>

int main()
{
	{
	std::vector<std::vector<std::array<std::size_t,3>>> arr{std::vector<std::array<std::size_t,3>>{std::array<std::size_t,3>{12142141,2,3},std::array<std::size_t,3>{4,5,6}}};
	fast_io::obuf out("binary_serialization.txt");
	write(out,arr);
	std::array<std::string,1> ar{"abcdef\n dsjfgoisdjgoisdjgdiso"};
	write(out,ar);
	}
	{
	std::deque<std::vector<std::array<std::size_t,3>>> arr;
	fast_io::ibuf in("binary_serialization.txt");
	read(in,arr);
	for(auto const& e : arr)
		for(auto const& e1:e)
			for(auto const & e2 : e1)
				fast_io::out<<e2<<"\n";
	std::string str;
	read(in,str);
	fast_io::out<<str<<"\n";
	}
}