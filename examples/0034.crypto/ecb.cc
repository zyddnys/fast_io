
#include"../../include/fast_io.h"

#include <array>
#include <string>

int main()
try
{
    std::array<uint8_t, 32> key = {'8','3','3','4',';','2','3','4','a','2','c','4',']','0','3','4','8','3','3','4',';','2','3','4','a','2','c','4',']','0','3','4'};
    {
        fast_io::obuf ob("ecb.out");
        fast_io::crypto::basic_oecb<decltype(ob), fast_io::crypto::speck::speck_enc_128_256> enc_stream(key, ob);
        //print(enc_stream, "abcdefghijklmnop1234567812345678ponmlkjihgfedcba");
        print(enc_stream, "abcde");
        print(enc_stream, "fghijklmnop1234567812345678ponmlkjihgfedcb");
        print(enc_stream, "a");
    }
    
    {
        fast_io::ibuf ib("ecb.out");
        fast_io::crypto::basic_iecb<decltype(ib), fast_io::crypto::speck::speck_dec_128_256> dec_stream(key, ib);
        transmit(fast_io::out, dec_stream);
    }
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}
