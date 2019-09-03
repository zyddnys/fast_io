#include"../../include/fast_io.h"

int main()
try
{
	{
	fast_io::obuf file("binary.txt");
	print(file,"hello world\n");
	}
	{
	fast_io::obuf file("text.txt");
	fast_io::text_view view(file);
	print(view,"hello world\n");
	}
}
catch(std::exception const & e)
{
	println(fast_io::out,e);
	return 1;
}