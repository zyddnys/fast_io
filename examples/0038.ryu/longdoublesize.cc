#include"../../include/fast_io.h"

int main()
{
	fprint(fast_io::out,"sizeof(double)==%\tsizeof(long double)==%\n",sizeof(double),sizeof(long double));
}