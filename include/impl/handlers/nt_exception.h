#pragma once
#include<stdexcept>
#include<array>

namespace fast_io
{

namespace details
{

inline constexpr auto generate_nt_exceptions()
{
	struct field
	{
		int ntstatus;
		int win32;
		int posix;
		char const* message;
	};
	field constexpr nt_error_category_array[] =
	{
#include"ntkernel-table.ipp"
	};
	std::array<std::pair<std::int32_t,char const*>,std::size(nt_error_category_array)> array{};
	for(std::size_t i(0);i!=array.size();++i)
	{
		auto & e(nt_error_category_array[i]);
		array[i].first=static_cast<std::int32_t>(e.ntstatus);
		array[i].second=e.message;
	}
	return array;
}

auto constexpr nt_exceptions{generate_nt_exceptions()};

inline constexpr char const* nt_exception_information(std::int32_t status)
{
	if(!status)
		return "The NT operation completed successfully";
	for(auto const & e : nt_exceptions)
		if(e.first == status)
			return e.second;
	return "Unknown NT status code";
}

}

class nt_error:public std::runtime_error
{
	std::int32_t status_val;
public:
	explicit nt_error(std::int32_t ntstatus):std::runtime_error(details::nt_exception_information(ntstatus)),status_val(ntstatus){}
	auto status() const {return status_val;}
};

}