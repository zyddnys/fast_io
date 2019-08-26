#include"../../include/fast_io.h"

int main()
{
	fast_io::out<<3<<'\n';//WRONG. character is treated in fast_io library as integer by default. '\n' would be treated as its ANSI value
// The reason is that std::int8_t/std::uint8_t would be treated as integer in iostream which breaks a lot of generic programming code. I decide to NEVER support shit like this.

/*
Correct examples:
	println(fast_io::out,3);

	fast_io::out<<3<<"\n";	

	(fast_io::out<<3).put('\n');

	fprint(fast_io::out,"%\n",3);	

	fast_io::out<<3<<fast_io::character('\n');

	print(fast_io::out,3,fast_io::character('\n'));
*/
}