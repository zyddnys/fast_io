#include"../../include/fast_io.h"

int main()
try
{
	fast_io::ibuf binary_file("binary.txt");
	std::string binary_file_content;
	getwhole(binary_file,binary_file_content);
	fast_io::text_view<fast_io::ibuf> text_file("text.txt");
	std::string text_file_content;
	getwhole(text_file,text_file_content);
	println(fast_io::out,"same? ",binary_file_content==text_file_content);
}
catch(std::exception const & e)
{
	println(fast_io::out,e);
	return 1;
}