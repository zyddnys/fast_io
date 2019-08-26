#include"../include/fast_io.h"

int main()
{
	fast_io::fprint(fast_io::out,"%\n%\n",fast_io::to<std::size_t>("325d"),
		fast_io::to<std::size_t>("ddd"));//try to convert from an invalid string
}