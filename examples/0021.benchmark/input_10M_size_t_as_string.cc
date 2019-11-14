#include"timer.h"
#include<fstream>
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<exception>
#include<string>
#include<vector>

int main()
{
	{
		cqw::timer t("std::ifstream");
		std::ifstream fin("cfilestar.txt",std::ifstream::binary);
		std::size_t value(0);
		for(;;)
		{
			std::string str;
			if(!(fin>>str))
				break;
			value+=str.size();
		}
		println(fast_io::out,"ifstream:",value);
	}
	{
		cqw::timer t("ibuf");
		fast_io::ibuf ibuf("cfilestar.txt");
		std::size_t value(0);
		try
		{
		for(;;)
		{
			std::string str;
			scan(ibuf,str);
			value+=str.size();
		}
		}
		catch(fast_io::eof const&){}
		println(fast_io::out,"ibuf:",value);
	}
}