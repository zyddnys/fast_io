#include"../../include/fast_io.h"

int main()
{
	fprint(fast_io::out,"Hello World%%%%\n");	//not throw exception since it matches
	fprint(fast_io::out,"Hello World%%%\n",3);
	fprint(fast_io::out,"Hello World%%%\n");	//throw exception since it does not match
}