#include<utility>
#include<iostream>
inline constexpr void print_define(int a)
{

}

template<typename T>
concept printable=requires(T&& t)
{
	print_define(std::forward<T>(t));
};

template<typename... Args>
requires (...&&printable<Args>)
inline constexpr void printd(Args&& ...args)
{
}

int main()
{
	printd();
//	printd(5,"6");
//	www(5,"asf");
}