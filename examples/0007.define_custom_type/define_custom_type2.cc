#include"../../include/fast_io.h"

struct foo
{
	std::string str;
	std::size_t t=0;
};

//fast_io::output_stream is a concept. NOT A TYPE. This is GP not OOP!
//Define this stream just like define operator<< for iostreams
// this needs standard_output_stream since output_stream does not guarantee your type can output character
inline void print(fast_io::standard_output_stream& out,foo const& d)
{
	print(out,d.str,fast_io::char_view('\t'),d.t);
}

inline constexpr fast_io::standard_input_stream& operator>>(fast_io::standard_input_stream& out,foo& d)
{
	print(out,d.str,fast_io::char_view('\t'),d.t);
}

int main()
{
	foo f{"abc",5};
	println(fast_io::out,f);
}