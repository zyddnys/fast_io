#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_crypto.h"
#include"../../include/fast_io_network.h"

int main()
try
{
	std::array<uint8_t, 16> key = {0xaf, 0xdd, 0xd3, 0x8e, 0xf9, 0xef, 0x27, 0x1a, 0x29, 0xf9, 0x05, 0x5d, 0x9e, 0x15, 0xee, 0x05};
	std::array<uint8_t, 16> iv = {0xb2, 0x6d, 0x69, 0xcc, 0xde, 0x21, 0xa0, 0xe7, 0x8a, 0x21, 0x2f, 0x86, 0x4a, 0xb4, 0x2d, 0x6c};
	fast_io::crypto::basic_icbc<fast_io::client, fast_io::crypto::speck::speck_dec_128_128>
			encrypted_hd(key, iv, fast_io::to<fast_io::ipv4>("127.0.0.1"),2000,fast_io::sock::type::stream);
	transmit(fast_io::out,encrypted_hd);
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}