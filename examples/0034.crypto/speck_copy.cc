#include"../../include/fast_io.h"
#include<exception>
#include<chrono>
#include<string_view>

int main(int argc,char **argv)
try
{	
	if(argc!=3)
	{
		fprint(fast_io::err,"Usage: % <source> <dest>\n",*argv);
		return 1;
	}
	std::array<uint8_t, 8> constexpr nonce{'1','2','3','4','1','2','3','4'};
    std::array<uint8_t, 16> constexpr key{'8','3','3','4',';','2','3','4','a','2','c','4',']','0','3','4'};
	std::string_view source_filename(argv[1]),dest_filename(argv[2]);
	auto t0(std::chrono::high_resolution_clock::now());
	std::size_t transmitted_bytes(0);
	{
		fast_io::ibuf source(source_filename);
		fast_io::nobuf_reader<fast_io::crypto::basic_ictr<decltype(source), fast_io::crypto::speck::speck_enc_128_128>> dec_stream(key, nonce, source);
		fast_io::obuf dest(dest_filename);
        fast_io::nobuf_writer<fast_io::crypto::basic_octr<decltype(dest), fast_io::crypto::speck::speck_enc_128_128>> enc_stream(key, nonce, dest);
		transmitted_bytes=transmit(enc_stream,dec_stream);
	}
	fprint(fast_io::out,"Copy % --> % Done (% bytes)\nTime elasped: %s\n",
				source_filename,dest_filename,transmitted_bytes,
				fast_io::fixed(std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now()-t0).count(),8));
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}
