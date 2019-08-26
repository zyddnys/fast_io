#include<iostream>
#include"../../include/fast_io.h"
#include<string>

struct foo
{
	std::string str;
	std::vector<std::size_t> vec;
};

// standard_input_stream is a CONCEPT. THIS IS NOT OBJECT ORIENTED PROGRAMMING!
// code is very similiar to how operator>> are defined. read means to machine friendly while operator>> is to be human readable.
inline constexpr fast_io::standard_input_stream& read(fast_io::standard_input_stream& in, foo& f)
{
// most standard library containers can all be directly read/write
	return read(in,f.str,f.vec);
}

inline constexpr fast_io::standard_output_stream& write(fast_io::standard_output_stream& out, foo const& f)
{
//Their order must be same with read
	return write(out,f.str,f.vec);
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
	fast_io::out<<"str: "<<f.str<<"\nvec:";
	for(auto const & e : f.vec)
		(fast_io::out<<e).put('\t');
	fast_io::out.put('\n');
	}
}