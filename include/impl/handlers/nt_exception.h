#pragma once
#include<stdexcept>
#include<algorithm>

namespace fast_io
{

namespace details
{

struct exception_info
{
	std::uint32_t ntstatus_code;
	char const* info_cstr;
};

inline bool operator<(exception_info const& a,exception_info const& b)
{
	return a.ntstatus_code<b.ntstatus_code;
}

inline bool operator<(exception_info const& a,std::uint32_t const& b)
{
	return a.ntstatus_code<b;
}

inline bool operator<(std::uint32_t const& a,exception_info const& b)
{
	return a<b.ntstatus_code;
}

inline exception_info constexpr nt_exceptions[]
{
#include"ntstatus_table.h"
};

inline constexpr char const* nt_exception_information(std::uint32_t status)
{
	if(!status)
		return "The NT operation completed successfully";
	auto it(std::lower_bound(std::cbegin(nt_exceptions),std::cend(nt_exceptions),status));
	if(it==std::cend(nt_exceptions))
		return "Unknown NT status code";
	else
		return it->info_cstr;
}

}

class nt_error:public std::runtime_error
{
	std::uint32_t status_val;
public:
	explicit nt_error(std::uint32_t ntstatus):std::runtime_error(details::nt_exception_information(ntstatus)),status_val(ntstatus){}
	auto status() const {return status_val;}
};

}