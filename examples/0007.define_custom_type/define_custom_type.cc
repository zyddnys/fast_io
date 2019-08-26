#include"../../include/fast_io.h"

struct foo
{
	std::string str;
};

//fast_io::output_stream is a concept. NOT A TYPE. This is GP not OOP!
//Define this stream just like define operator<< for iostreams
inline constexpr fast_io::output_stream& operator<<(fast_io::output_stream& out,foo const& d)
{
	return out<<d.str;
}

int main()
{
	foo f{"abc"};
	fast_io::out<<f<<"\n";
}