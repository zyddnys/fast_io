#include"../../include/fast_io.h"
#include<string>

int main()
{
	std::string str;
	for(int ch;(ch=get(fast_io::in))!=EOF;str.push_back(ch));
// WRONG!!! EOF does not exist in fast_io library. Nor char_traits!!!! It will throw exception when it reaches EOF!!!
// correct example see eof_correct.cc
}