#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"
#include"../../include/fast_io.h"

int main()
try
{
	fast_io::client_buf hd(fast_io::dns_once("www.jszg.edu.cn"),80,fast_io::sock::type::stream);
	fast_io::isystem_file request_file("http_request.txt");
	transmit(hd,request_file);
	skip_http_header(hd);
	fast_io::osystem_file osf("index.html");
	transmit(osf,hd);
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}