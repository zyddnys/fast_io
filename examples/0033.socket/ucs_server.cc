#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_crypto.h"
#include"../../include/fast_io_network.h"

int main()
try
{
	fast_io::server hd(2000,fast_io::sock::type::stream);
	for(;;)
	try
	{
		fast_io::ucs<fast_io::acceptor_buf,char32_t> accept(hd);
		print(fast_io::err,"a client connect to this server\n");
		print_flush(accept,U"☭🚄😀我wubi😄😊😉😍😘😚😜😝😳😁😣😢😂😭😪😥😰😩㍿🀀🀁🀂🀃🀄🀅🀆🀇🀈🀉🀊🀋🀌🀍🀎🀏");
	}
	catch(std::exception const& e)
	{
		println(fast_io::err,e);
	}
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}