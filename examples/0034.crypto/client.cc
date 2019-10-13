#include"../../include/fast_io.h"

int main()
try
{
	fast_io::client_buf hd(fast_io::sock::family::ipv4,fast_io::address(2000,"127.0.0.1"),fast_io::sock::type::stream);
	std::array<uint8_t, 16> key = {0xaf, 0xdd, 0xd3, 0x8e, 0xf9, 0xef, 0x27, 0x1a, 0x29, 0xf9, 0x05, 0x5d, 0x9e, 0x15, 0xee, 0x05};
	std::array<uint8_t, 16> iv = {0xb2, 0x6d, 0x69, 0xcc, 0xde, 0x21, 0xa0, 0xe7, 0x8a, 0x21, 0x2f, 0x86, 0x4a, 0xb4, 0x2d, 0x6c};
	std::string str;
	fast_io::crypto::basic_icbc<decltype(hd), fast_io::crypto::speck::speck_dec_128_128, 16, 16> encrypted_hd(key, iv, hd);
	getwhole(encrypted_hd,str);
	println(fast_io::out,str);
}
catch(std::system_error const & e)
{
	println(fast_io::err,e);
	return 1;
}