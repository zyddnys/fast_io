#include<iostream>
#include<iomanip>
#include"handlers/c_style.h"
#include"wrapper.h"
#include"iomutex.h"
#include"iobuf.h"
#include"stringbuf.h"
#include<string>
#include"read_write.h"
#include<array>
#include<vector>
#include<deque>
#include<list>
#include<unordered_set>

int main()
{
	using namespace fast_io;
	{
	std::vector<std::array<std::array<std::size_t,3>,2>> arr{std::array<std::array<std::size_t,3>,2>{std::array<std::size_t,3>{12142141,2,3},std::array<std::size_t,3>{4,5,6}}};
	output_wrapper<c_style_file> out("a.txt");
	write(out,arr);
	std::array<std::string,1> ar{"abcdef\n dsjfgoisdjgoisdjgdiso"};
	write(out,ar);
	std::vector<std::deque<std::list<std::array<std::unordered_set<std::size_t>,3>>>> lol;
	write(out,lol);
	}
	{
	std::deque<std::array<std::array<std::size_t,3>,2>> arr;
	input_wrapper<c_style_file> in("a.txt");
	read(in,arr);
	for(auto const& e : arr)
		for(auto const& e1:e)
			for(auto const & e2 : e1)
				std::cout<<e2<<'\n';
	std::string str;
	read(in,str);
	std::cout<<str<<'\n';
	}
}