#include"../../include/fast_io.h"
#include<string>
#include<vector>

struct foo
{
	std::string str;
	std::vector<std::size_t> vec;
};

// character_input_stream is a CONCEPT. THIS IS NOT OBJECT ORIENTED PROGRAMMING!
template<fast_io::character_input_stream input>
inline constexpr void read(input& in, foo& f)
{
// most standard library containers can all be directly read/write
	read(in,f.str,f.vec);
}

template<fast_io::character_output_stream output>
inline constexpr void write(output& out, foo const& f)
{
//Their order must be same with read
	write(out,f.str,f.vec);
}

int main()
{
//write data to file (Serialization)
	{
	foo f{"abcdef",std::vector<std::size_t>(5,6)};
	fast_io::obuf_mutex ob("custom_binary_serialization_type.txt");
	write(ob,f);
	}
//read data from file (Deserialization)
	{
	fast_io::ibuf ib("custom_binary_serialization_type.txt");
	foo f;
	read(ib,f);

//output content
	print(fast_io::out,"str: ",f.str,"\nvec:");
	for(auto const & e : f.vec)
		print(fast_io::out,e,"\t");
	println(fast_io::out);
	}
}