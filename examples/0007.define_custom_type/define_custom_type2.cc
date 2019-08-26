#include"../../include/fast_io.h"

struct foo
{
	std::string str;
	std::size_t t=0;
};

//fast_io::output_stream is a concept. NOT A TYPE. This is GP not OOP!
//Define this stream just like define operator<< for iostreams
// this needs standard_output_stream since output_stream does not guarantee your type can output character
inline constexpr fast_io::standard_output_stream& operator<<(fast_io::standard_output_stream& out,foo const& d)
{
	return out<<d.str<<"\t"<<d.t;
}

inline constexpr fast_io::standard_input_stream& operator>>(fast_io::standard_input_stream& out,foo& d)
{
	return out>>d.str>>d.t;
}

int main()
{
	foo f{"abc",5};
	fast_io::out<<f<<"\n";
}