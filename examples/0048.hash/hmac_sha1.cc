#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_hash.h"

int main()
try
{
	fast_io::hmac_sha1 hmacs1("123452351532319875129385712389547192834791832ru89231ur98217r9123yr29813ry89123ry89123yr98123ry132r12");
	print(hmacs1,"abc");
	flush(hmacs1);
	println(fast_io::out,hmacs1);
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}