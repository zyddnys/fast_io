#include"timer.h"
#include<fstream>
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_crypto.h"
#include"../../include/fast_io_c_interface.h"
#include<exception>
#include<cmath>
#include<memory>
#include<cstdio>
#include<charconv>

int main()
try
{
	std::size_t constexpr N(10000000);
	{
	cqw::timer t("std::FILE*");
	std::unique_ptr<std::FILE,decltype(fclose)*> fp(std::fopen("cfilestar.txt","wb"),fclose);
	for(std::size_t i(0);i!=N;++i)
		fprintf(fp.get(),"%zu\n",i);
	}
	{
	cqw::timer t("std::ofstream");
	std::ofstream fout("ofstream.txt",std::ofstream::binary);
	for(std::size_t i(0);i!=N;++i)
		fout<<i<<'\n';
	}
	{
	cqw::timer t("std::ofstream with tricks");
	std::ofstream fout("ofstream_tricks.txt",std::ofstream::binary);
	auto &rdbuf(*fout.rdbuf());
	for(std::size_t i(0);i!=N;++i)
	{
		fout<<i;
		rdbuf.sputc('\n');
	}
	}
	{
	cqw::timer t("std::to_chars + ofstream rdbuf tricks");
	std::ofstream fout("ofstream_to_chars_tricks.txt",std::ofstream::binary);
	auto &rdbuf(*fout.rdbuf());
	std::array<char, 50> buffer;
	for(std::size_t i(0);i!=N;++i)
	{
		auto [p,ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(),i);
		*p='\n';
		rdbuf.sputn(buffer.data(),++p-buffer.data());
	}
	}
	{
	cqw::timer t("std::to_chars + obuf");
	fast_io::obuf obuf("std_to_chars_obuf.txt");
	std::array<char, 50> buffer;
	for(std::size_t i(0);i!=N;++i)
	{
		auto [p,ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(),i);
		*p='\n';
		send(obuf,buffer.data(),++p);
//		put(obuf,'\n');
	}
	}
	{
	cqw::timer t("obuf");
	fast_io::obuf obuf("obuf.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,i);
	}
	{
	cqw::timer t("fast_io c interface");
	void *handle;
	cxx_fast_io_acquire_buf(std::addressof(handle),"c_interface.txt","wb");
	for(std::size_t i(0);i!=N;++i)
	{
		cxx_fast_io_println_size_t(handle,i);
//		cxx_fast_io_print_c_str(handle,"\n");
	}
	cxx_fast_io_release(handle);
	}
	{
	cqw::timer t("fast_io c interface bufferred");
	void *handle;
	cxx_fast_io_bufferred_acquire_file(std::addressof(handle),"c_interfacebufferred.txt","wb");
	for(std::size_t i(0);i!=N;++i)
	{
		cxx_fast_io_bufferred_println_size_t(handle,i);
//		cxx_fast_io_bufferred_put(handle,'\n');
	}
	cxx_fast_io_bufferred_release(handle);
	}

}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}