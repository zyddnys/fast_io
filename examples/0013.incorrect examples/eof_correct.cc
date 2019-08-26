#include"../../include/fast_io.h"
#include<string>

int main()
{
	std::string str;
	for(std::pair<char,bool> ch;!((ch=fast_io::in.try_get()).second);str.push_back(ch));

/*
try_get() returns 2 value. bool represents whether it reaches EOF. true means it reaches EOF.
When it reaches EOF. the first member of the pair is guarantee to be 0.

However. EOF in a lot of cases are useless. You should use get() instead to prevent EOF hacking.

get() will throw exception including EOF
try_get() will throw exception excluding EOF
*/
}