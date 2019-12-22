#pragma once

namespace fast_io
{
class integer
{
	natural n;
	bool sign=false;
public:
	integer()=default;
	template<typename T>
	requires std::unsigned_integral<T>
	explicit integer(T value):n(value){}
	template<typename T>
	requires std::signed_integral<T>
	explicit integer(T value)
	{
		if(value<0)
		{
			sign=true;
			if(value==std::numeric_limits<T>::min())
			{
				n=natural(-(value+1))+1;
				return;
			}
			value=-value;
		}
		n=natural(value);
	}
	template<typename T>
	requires std::same_as<T,natural>
	explicit integer(T&& value):n(std::forward<T>(value)){}
	auto& natural() {return n;}
	auto& natural() const{return n;}
	template<typename T>
	requires std::unsigned_integral<T>
	inline integer& operator+=(T u)
	{
		if(sign)
		{
			if(n<u)
			{
				n=natural(u-n);
				sign=true;
			}
			else
				n-=u;
		}
		n+=u;
		return *this;
	}
	inline integer& operator+=(natural const &u)
	{
		if(sign)
		{
			if(n<u)
			{
				n=u-n;
				sign=true;
			}
			else
				n-=u;
		}
		n+=u;
		return *this;
	}

	template<typename T>
	requires std::signed_integral<T>
	inline integer& operator+=(T value)
	{
		if(value<0)
		{
			if(value==std::numeric_limits<T>::min())
			{
				value=-(value+1);
				if(sign)
				{
					n+=value;
					++n;
				}
				else
				{
					n-=value;
					--n;
				}
				return *this;
			}
			value=-value;
			if(sign)
				n+=value;
			else
				n-=value;
			return *this;
		}
		return *this+=static_cast<std::make_unsigned_t<T>>(value);
	}
	inline integer operator-()
	{
		integer temp(*this);
		temp.sign = not temp.sign;
		return temp;
	}
	inline integer operator+()
	{
		return *this;
	}
	inline bool negative() const
	{
		return sign;
	}
	inline integer& operator+=(integer const& value)
	{
		if (sign ^ value.sign)
		{
			if (value.n > n)
			{
				n = value.n - n;
				sign = !sign;
			}
			else
			{
				n -= value.n;
			}
			return *this;
		}
		n += value.n;
		return *this;
	}

	inline integer& operator-=(integer const& value)
	{
		if (sign ^ !value.sign)
		{
			if (value.n > n)
			{
				n = value.n - n;
				sign = !sign;
			}
			else
			{
				n -= value.n;
			}
			return *this;
		}
		n += value.n;
		return *this;
	}
	inline integer& operator*=(natural const& value)
	{
		n%=value;
		return *this;
	}
	inline integer& operator/=(natural const& value)
	{
		n/=value;
		return *this;
	}
	inline integer& operator%=(natural const& value)
	{
		n%=value.n;
		return *this;
	}
	inline integer& operator*=(integer const& value)
	{
		sign^=value.sign;
		n%=value.n;
		return *this;
	}
	inline integer& operator/=(integer const& value)
	{
		sign^=value.sign;
		n/=value.n;
		return *this;
	}
	inline integer& operator%=(integer const& value)
	{
		n%=value.n;
		return *this;
	}
};

}