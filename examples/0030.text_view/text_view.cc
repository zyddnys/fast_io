#include"../../include/fast_io.h"

int main()
try
{
	{
	fast_io::obuf file("binary.txt");
	println(file,"hello world\nmmd\n");
	}
	{
	fast_io::obuf file("text.txt");
	fast_io::text_view view(file);
	println(view,"hello world\nmmd\n");
	}
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}