#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"
#include"../../include/fast_io_hash.h"
#include"domain.h"
#include<random>
#include"key_info.h"

int main(int argc, char** argv)
try
{
	if(argc!=3)
	{
		println(fast_io::err,"Usage: ",*argv," <serial> <restore code>");
		return 1;
	}
	std::string serial(battlenet::normalize_serial(argv[1]));
	if(serial.size()!=14)
	{
		println(fast_io::err,"Serial should be 14 bytes after normalize");
		return 1;
	}
	std::string restore_str(battlenet::normalize_serial(argv[2]));
	if(restore_str.size()!=10)
	{
		println(fast_io::err,"Restore should be 10 bytes");
		return 1;
	}
	for(auto & e : restore_str)
		e=std::to_integer<char>(battlenet::char_to_byte(e));
	auto const region_array(battlenet::upper_case_region({serial.data(),2}));
	std::string_view const region(region_array.data(),region_array.size());
	auto const domain(battlenet::get_domain(region));
	std::array<std::uint8_t,32> challenge;
	auto dns_cache(fast_io::dns_once(domain));
	{
	fast_io::client_buf hd(dns_cache,80,fast_io::sock::type::stream);
	print(hd,"POST ",battlenet::restore_path," HTTP/1.1\r\n",
		"Host: ",domain,"\r\n"
		"Content-Type: application/octet-stream\r\n"
		"Content-Length: 14\r\n"
		"Timeout: 120\r\n\r\n");
	skip_http_header(hd);
	receive(hd,challenge.begin(),challenge.end());
	}
	fast_io::hmac_sha1 hmac_sha1(restore_str);
	send(hmac_sha1,serial.cbegin(),serial.cend());
	write(hmac_sha1,challenge.cbegin(),challenge.cend());
	flush(hmac_sha1);
	decltype(auto) hmac_digest{get_digest(hmac_sha1)};

	std::random_device device;
	std::uniform_int_distribution<std::uint8_t> dis(0,255);

	std::array<std::uint8_t,20> one_time_pad_key;
	for(auto &e : one_time_pad_key)
		e=dis(device);

	fast_io::ostring challenge_ostr;
	write(challenge_ostr,hmac_digest);
	write(challenge_ostr,one_time_pad_key);
	fast_io::natural modules;
	{
	fast_io::istring_view isv("955e4bd989f3917d2f15544a7e0504eb9d7bb66b6f8a2fe470e453c779200e5e"
			"3ad2e43a02d06c4adbd8d328f1a426b83658e88bfd949b2af4eaf30054673a14"
			"19a250fa4cc1278d12855b5b25818d162c6e6ee2ab4a350d401d78f6ddb99711"
			"e72626b48bd8b5b0b7f3acf9ea3c9e0005fee59e19136cdb7c83f2ab8b0a2a99");
	scan(isv,fast_io::hex(modules));
	}
	fast_io::natural exponent("257");
	fast_io::natural p;
	p.vec().resize(challenge_ostr.str().size()%8?challenge_ostr.str().size()/8+1:
		challenge_ostr.str().size()/8);
	std::memcpy(p.vec().data(),challenge_ostr.str().data(),challenge_ostr.str().size());
	auto result(pow_mod(p,exponent,modules));
	{
	fast_io::client_buf hd(dns_cache,80,fast_io::sock::type::stream);
	print(hd,"POST ",battlenet::restore_validate_path," HTTP/1.1\r\n",
		"Host: ",domain,"\r\n"
		"Content-Type: application/octet-stream\r\n"
		"Content-Length: ",serial.size()+p.vec().size()*8,"\r\n"
		"Timeout: 1000\r\n\r\n");
	send(hd,serial.cbegin(),serial.cend());
	send(hd,p.vec().cbegin(),p.vec().cend());
/*	skip_http_header(hd);
	std::array<std::uint8_t,20> ret;
	read(hd,ret);
	for(std::size_t i(0);i!=ret.size();++i)
		ret[i]^=one_time_pad_key[i];*/
	
	fast_io::osystem_file osyshandle("restore_validate_temp.txt");
	transmit(osyshandle,hd);	
	}


}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}