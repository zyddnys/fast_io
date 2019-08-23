#pragma once
#include<stdexcept>
#include<string_view>

namespace fast_io
{
enum class openmode:std::uint32_t
{
in=1,
out=2,
app=4,
ate=8,
trunc=16,
no_overwrite=32//C++ iostream currently still does not support "x"
//no binary since binary is ignored on most platforms.
};

inline constexpr openmode& operator|=(openmode& a,openmode const& b)
{
	return (a=static_cast<openmode>(static_cast<std::uint32_t>(a)|static_cast<std::uint32_t>(b)));
}

inline constexpr openmode operator|(openmode const& a,openmode const& b)
{
	auto temp(a);
	temp|=b;
	return temp;
}


template<openmode om>
struct open_interface_t
{
constexpr open_interface_t()
{
//https://en.cppreference.com/w/cpp/io/basic_filebuf/open
	switch(static_cast<openmode>(static_cast<std::uint32_t>(om)&~static_cast<std::uint32_t>(openmode::ate)))
	{
//Action if file already exists;	Action if file does not exist;	c-style mode;	Explanation
//Read from start;	Failure to open;	"r";	Open a file for reading
	case openmode::in:
//Destroy contents;	Create new;	"w";	Create a file for writing
	case openmode::out:
	case openmode::out|openmode::trunc:
//Append to file;	Create new;	"a";	Append to a file
	case openmode::app:
	case openmode::out|openmode::app:
//Read from start;	Error;	"r+";		Open a file for read/write
	case openmode::out|openmode::in:
//Destroy contents;	Create new;	"w+";	Create a file for read/write
	case openmode::out|openmode::in|openmode::trunc:
//Write to end;	Create new;	"a+";	Open a file for read/write
	case openmode::out|openmode::in|openmode::app:
	case openmode::in|openmode::app:

//Destroy contents;	Error;	"wx";	Create a file for writing
	case openmode::out|openmode::no_overwrite:
	case openmode::out|openmode::trunc|openmode::no_overwrite:
//Append to file;	Error;	"ax";	Append to a file
	case openmode::app|openmode::no_overwrite:
	case openmode::out|openmode::app|openmode::no_overwrite:
//Destroy contents;	Error;	"w+x";	Create a file for read/write
	case openmode::out|openmode::in|openmode::trunc|openmode::no_overwrite:
//Write to end;	Error;	"a+x";	Open a file for read/write
	case openmode::out|openmode::in|openmode::app|openmode::no_overwrite:
	case openmode::in|openmode::app|openmode::no_overwrite:
	break;
	default:
		throw std::runtime_error("unknown open mode");
	}
}
};

template<openmode om>
inline open_interface_t<om> constexpr open_interface;

auto constexpr c_style_openmode(std::string_view c_style_mode)
{
	openmode v{};
	bool extended(false);
	for(auto const& e : c_style_mode)
		if(e=='+')
			extended=true;
	for(auto const& e : c_style_mode)
		switch(e)
		{
			case 'r':
				v|=openmode::in;
				if(extended)
					v|=openmode::out;
			break;
			case 'w':
				v|=openmode::out;
				if(extended)
					v|=openmode::in|openmode::trunc;
			break;
			case 'a':
				v|=openmode::app;
				if(extended)
					v|=openmode::in|openmode::out;
			break;
			case 'x':
				v|=openmode::no_overwrite;
			break;
			case '+':
			break;
			case 'b':
			break;//ignore 'b'
			default:
				throw std::runtime_error("unknown C-style open mode");
		}
	return v;
}

class native_interface_t{}native_interface;
}