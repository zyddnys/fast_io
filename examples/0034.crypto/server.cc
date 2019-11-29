#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_crypto.h"
#include"../../include/fast_io_network.h"

int main()
try
{
	fast_io::server hd(2000,fast_io::sock::type::stream);
	std::array<uint8_t, 16> key = {0xaf, 0xdd, 0xd3, 0x8e, 0xf9, 0xef, 0x27, 0x1a, 0x29, 0xf9, 0x05, 0x5d, 0x9e, 0x15, 0xee, 0x05};
	std::array<uint8_t, 16> iv = {0xb2, 0x6d, 0x69, 0xcc, 0xde, 0x21, 0xa0, 0xe7, 0x8a, 0x21, 0x2f, 0x86, 0x4a, 0xb4, 0x2d, 0x6c};
	fast_io::osystem_file log_file("error_log.txt");
	for(;;)
	try
	{
		fast_io::crypto::basic_ocbc<fast_io::acceptor_buf, fast_io::crypto::speck::speck_enc_128_128> encrypted_accept(key, iv, hd);
		print(encrypted_accept,"hello world\n");
	}
	catch(std::exception const& e)
	{
		println(log_file,e);//atomic log. You do not need to worry about thread/process safety
	}
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}