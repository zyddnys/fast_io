#include"../../include/fast_io.h"

int main()
try
{
	fast_io::server hd(fast_io::sock::family::ipv4,fast_io::address(2000),fast_io::sock::type::stream);
	for(;;)
	{
		fast_io::ucs<fast_io::acceptor_buf,char32_t> accept(hd);
		print(accept,U"☭🚄😀我wubi😄😊😉😍😘😚😜😝😳😁😣😢😂😭😪😥😰😩㍿🀀🀁🀂🀃🀄🀅🀆🀇🀈🀉🀊🀋🀌🀍🀎🀏");
		println_flush(fast_io::log,"a client connect to this server");
	}
}
catch(std::system_error const & e)
{
	println(fast_io::err,e);
	return 1;
}