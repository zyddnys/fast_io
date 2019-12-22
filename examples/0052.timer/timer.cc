#include"../../include/fast_io.h"

class timer
{
	std::string s;
	std::chrono::high_resolution_clock::time_point t0;
public:
	template<typename ...Args>
	requires std::constructible_from<std::string,Args...>
	timer(Args&& ...args):s(std::forward<Args>(args)...),t0(std::chrono::high_resolution_clock::now()){}
	timer(const timer&) = delete;
	timer& operator=(const timer&) = delete;
	~timer()
	{
		fprint(fast_io::err,"%:\t%s\n",s,std::chrono::high_resolution_clock::now()-t0);
	}
};

int main(int argc,char** argv)
try
{
	if(argc<2)
	{
		println(fast_io::err,"Usage: ",*argv," <command>");
		return 1;
	}
	timer t(argv[1]);
	std::system(argv[1]);
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}