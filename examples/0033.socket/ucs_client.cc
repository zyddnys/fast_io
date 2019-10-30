#include"../../include/fast_io.h"

int main()
try
{
	fast_io::ucs<fast_io::client_buf,char32_t> hd(fast_io::sock::family::ipv4,fast_io::address(2000,"127.0.0.1"),fast_io::sock::type::stream);
	std::u32string str;
	getwhole(hd,str);
	fast_io::ucs<fast_io::io_ref<decltype(fast_io::out)>,char32_t> view(fast_io::out);
	println(view,str);
	for(auto const & e : str)
		fprint(fast_io::out," % (0x%)\n",fast_io::unsigned_view(e),fast_io::hexupper(fast_io::unsigned_view(e)));
}
catch(std::system_error const & e)
{
	println(fast_io::err,e);
	return 1;
}