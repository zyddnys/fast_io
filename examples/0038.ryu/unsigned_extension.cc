#include"../../include/fast_io.h"

int main()
{
	using namespace fast_io::literals;
	auto constexpr v("235987238957328957329587325987238957239532759832749832"_u1024*"235987238957328957329587325987238957239532759832749832"_u1024);
	println_flush(fast_io::out,v);
}