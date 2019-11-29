#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_crypto.h"
#include"../../include/fast_io_network.h"

int main()
try
{
	fast_io::ucs<fast_io::client_buf,char32_t> hd(fast_io::to<fast_io::ipv4>("127.0.0.1"),2000,fast_io::sock::type::stream);
	std::u32string str;
	getwhole(hd,str);
	fast_io::ucs<fast_io::basic_iobuf<fast_io::system_io_handle>,char32_t> view(fast_io::out);
	println(view,str);
	for(auto const & e : str)
		fprint(fast_io::out," % (0x%)\n",fast_io::unsigned_view(e),fast_io::hexupper(fast_io::unsigned_view(e)));
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}