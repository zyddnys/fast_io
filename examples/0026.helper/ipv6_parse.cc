#include"../../include/fast_io.h"

inline auto ipv6_address(std::string_view str)
{
	fast_io::ostring ostr;
	std::size_t prefix_zero(0);
	for(auto it(str.cbegin());it!=str.cend();++it)
	{
		std::uint8_t e(*it);
		if(e==':')
		{
			if(it==str.cbegin())
				ostr.put('0');
			++prefix_zero;
			ostr.put(' ');
			if(it+1==str.cend())
				ostr.put('0');
			else if(it[1]==':')
			{
				ostr.put('0');
				auto j(it+1);
				for(;j!=str.cend();++j)
					if(*j==':')
						++prefix_zero;
				for(;prefix_zero<7;++prefix_zero)
				{
					ostr.put(' ');
					ostr.put('0');
				}
			}
		}
		else if(e-'0'<10||e-'a'<6||e-'A'<6)
			ostr.put(e);
		else
			break;
	}
	std::vector<std::uint16_t> value(8);
	fast_io::istring_view istrbuf(ostr.str());
	for(auto & e : value)
		scan(istrbuf,fast_io::hex(e));
	return value;
}

int main()
{
	for(auto const & e :ipv6_address("2001:db8::1"))
	{
		fprint(fast_io::out,"%\t",fast_io::hex(e));
	}
}