#include"../../include/fast_io.h"

struct foo
{
	std::string str;
};

//fast_io::output_stream is a concept. NOT A TYPE. This is GP not OOP!
//Define this stream just like define operator<< for iostreams
template<fast_io::output_stream output>
inline constexpr void print(output& out,foo const& d)
{
	print(out,d.str);
}

int main()
{
	foo f{"abc"};
	println(fast_io::out,f);
}