#include"../../include/fast_io.h"

struct foo
{
	std::string str;
	std::size_t t=0;
};

//fast_io::output_stream is a concept. NOT A TYPE. This is GP not OOP!
// this needs character_output_stream since output_stream does not guarantee your type can output character
template<fast_io::character_output_stream output>
inline constexpr void print(output& out,foo const& d)
{
	print(out,d.str,fast_io::char_view('\t'),d.t);
}

template<fast_io::character_input_stream input>
inline constexpr void scan(input& in,foo& d)
{
	scan(in,d.str);
}

int main()
{
	foo f{"",5};
	scan(fast_io::in,f);
	println(fast_io::out,f);
}