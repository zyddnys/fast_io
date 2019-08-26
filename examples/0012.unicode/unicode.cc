#include"../../include/fast_io.h"
int main()
{
	fast_io::obuf ob("unicode.txt");
	fast_io::unicode_view<decltype(ob),wchar_t> uv(ob);
	uv<<L"工要在地一，上是中国同和的有人我主产不为这经以发了民㍿dsgds\n";
}