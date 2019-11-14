#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
try
{
	{
	fast_io::obuf file("binary.txt");
	println(file,"hello world\nmmd\n");
	}
	{
	fast_io::text_view<fast_io::obuf> view("text.txt");
	println(view,"hello world\nmmd\n");
	}
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}