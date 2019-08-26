#include"../../include/fast_io.h"
int main()
{
	fast_io::obuf ob("utf8_to_unicode.txt");
	ob<<fast_io::unicode_to_utf8(fast_io::utf8_to_unicode("工要在地一，上是中国同和的有人我主产不为这经以发了民㍿dsgds\n"));
}