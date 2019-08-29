//test addressof
#include<array>
#include"../../include/fast_io.h"
#include<memory>

template<typename T>
inline constexpr T* my_addressof(T& arg) noexcept
{
	return __builtin_addressof(arg);
}

template<typename T>
T const* my_addressof(T const&&) = delete;

int main()
{
	std::size_t constexpr i(0);
	auto constexpr p(*my_addressof(i));
	//sounds like std::addressof does use magic from compiler itself
//	println(fast_io::out,my_addressof(i));
}