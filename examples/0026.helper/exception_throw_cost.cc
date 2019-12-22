#include"timer.h"
#include<exception>

class eof final:std::exception
{
public:
	explicit eof()=default;
	char const* what() const noexcept{return "EOF";}
};

int main()
{
	cqw::timer t("exception");
	for(std::size_t i(0);i!=1000000;++i)
		try
		{
			throw eof();
		}
		catch(eof const &){}
}