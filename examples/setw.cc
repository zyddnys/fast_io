#include"../include/fast_io.h"

int main()
{
	fprint(fast_io::out,"%\n%\n",fast_io::setw(5,"abc"),fast_io::setw(4,"def",'0'));
}

/*Output:
  abc
0def

*/