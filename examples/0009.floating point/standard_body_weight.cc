#include"../../include/fast_io.h"

int main()
try
{
	println(fast_io::out,"Please enter your height(cm): ");
	double height;
	scan(fast_io::in,height);
	print(fast_io::out,"Your standard body weight is :",(height-80)*0.7,"kg\n");
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}