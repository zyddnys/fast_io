#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"


int main()
try
{
	fast_io::client_buf hd(fast_io::dns_once("www.jszg.edu.cn"),80,fast_io::sock::type::stream);
	print(hd,"GET /portal/home/index HTTP/1.1\r\n"
		"Host:www.jszg.edu.cn\r\n"
		"User-agent:whatever\r\n"
		"Accept-Type:*/*\r\n"
		"Connection:close\r\n\r\n");
	skip_http_header(hd);
	fast_io::osystem_file osf("index.html");
	transmit(osf,hd);
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}