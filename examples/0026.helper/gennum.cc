#include<array>

inline constexpr std::pair<std::size_t,std::size_t> cal_base_pw_size(std::size_t u)
{
	std::size_t retch(0);
	std::size_t retpw(0);

	std::size_t chars(0);
	std::size_t pw(1);
	for(;pw*chars<4096;++chars)
	{
		retch=chars;
		retpw=pw;
		pw*=u;
	}
	return {retch,retpw};
}

template<std::size_t base,bool upper>
inline constexpr auto cal_content()
{
	constexpr auto val(cal_base_pw_size(base));
	constexpr std::size_t chars(val.first);
	constexpr std::size_t pw(val.second);
	std::array<std::array<char,chars>,pw> vals{};
	for(std::size_t i(1);i<pw;++i)
	{
		auto& val(vals[i]);
		val=vals[i-1];
		std::size_t j(chars);
		for(;j--;)
		{
			if(val[j]==base-1)
				val[j]=0;
			else
				break;
		}
		++val[j];
	}
	for(auto &e : vals)
		for(auto &e1 : e)
			if constexpr(base<10)
			{
				if(e1<10)
					e1+='0';
				else
				{
					if constexpr(upper)
						e1+='A'-10;
					else
						e1+='a'-10;
				}
			}
			else
				e1+='0';
	return vals;
}

template<std::size_t base,bool upper>
struct shared_static_base_table
{
	inline static constexpr auto table=cal_content<base,upper>();
};

int main()
{
}