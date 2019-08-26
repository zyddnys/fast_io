#include"../../include/fast_io.h"
int main()
{
	fast_io::obuf ob("unicode_to_utf8.txt");
	ob<<fast_io::unicode_to_utf8(L"工要在地一，上是中国同和的有人我主产不为这经以发了民㍿	😀wubi\n");
}