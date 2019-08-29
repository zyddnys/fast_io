#include"../../include/fast_io.h"

struct foo
{
	std::string str;
	std::size_t t=0;
};

//fast_io::output_stream is a concept. NOT A TYPE. This is GP not OOP!
// this needs standard_output_stream since output_stream does not guarantee your type can output character
inline constexpr void print(fast_io::standard_output_stream& out,foo const& d)
{
	print(out,d.str,fast_io::char_view('\t'),d.t);
}

inline constexpr void scan(fast_io::standard_input_stream& in,foo& d)
{
	scan(in,d.str);
}

int main()
{
	foo f{"",5};
	scan(fast_io::in,f);
	println(fast_io::out,f);
}