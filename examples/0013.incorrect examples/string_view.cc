#include"../../include/fast_io.h"
#include<string_view>

int main()
{
	std::string_view sv("abcdef.txtd");
	sv.remove_suffix(1);
	println(fast_io::out,sv);
	fast_io::obuf ob(sv);	//WRONG. This will probably read until '\0'.
}