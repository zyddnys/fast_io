#include"ryu.h"
#include"timer.h"

int main()
{

	{
		cqw::timer t("ryu default");
		std::size_t sum(0);
		for(std::size_t i(0);i!=100000000;++i)
			sum+=fast_io::ryu_details::decimal_length(i*i);
		println_flush(fast_io::out,"ryu default",sum);
	}
}