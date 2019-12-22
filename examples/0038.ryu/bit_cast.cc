#include<type_traits>
#include<cstdint>

#include<type_traits>
#include<iostream>
#include<cstring>



int main()
{
	std::uint64_t const value(bit_cast<std::uint64_t>(1.0));
	std::cout<<value<<'\n';
}