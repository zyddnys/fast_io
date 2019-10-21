
#include"../../include/fast_io.h"

#include <array>
#include <string>
#include<iterator>
#include<vector>
#include <deque>



int main()
try
{
	std::array<uint8_t, 8> nonce = {'1','2','3','4','1','2','3','4'};
	std::array<uint8_t, 16> key = {'8','3','3','4',';','2','3','4','a','2','c','4',']','0','3','4'};
	{
		fast_io::crypto::basic_octr<fast_io::obuf, fast_io::crypto::speck::speck_enc_128_128> enc_stream(key, nonce, "ctr.out");
		enc_stream.put('a');
		print(enc_stream, "bc");
		print(enc_stream, "defghijklmnop12345678123456");
		print(enc_stream, "78ponmlkjihgfedcba");
		print_flush(enc_stream, "xx\n");
	}
	
	{
		fast_io::crypto::basic_ictr<fast_io::ibuf, fast_io::crypto::speck::speck_enc_128_128> dec_stream(key, nonce, "ctr.out");
		transmit(fast_io::out, dec_stream);
	}
	
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}


