#pragma once

namespace battlenet
{
using namespace std::string_view_literals;

template<std::integral char_type>
inline constexpr char_type to_upper(char_type ch)
{
	if('a'<=ch&&ch<='z')
		return (ch-'a')+'A';
	return ch;
}

inline std::array<char,2> upper_case_region(std::string_view region)
{
	if(region.size()!=2)
		throw std::runtime_error(fast_io::concat<>("Unknown Region: "sv,region));
	return {to_upper(region.front()),to_upper(region.back())};
}

inline constexpr std::string_view get_domain(std::string_view region)
{
	if(region=="CN"sv)
		return "mobile-service.battlenet.com.cn"sv;
/*	else if(region=="EU"sv)
		return "m.eu.mobileservice.blizzard.com"sv;
	else if(region=="US"sv)
		return "m.us.mobileservice.blizzard.com"sv;*/
	return "mobile-service.blizzard.com"sv;
}

inline constexpr auto enroll_path("/enrollment/enroll2.htm"sv);
inline constexpr auto sync_path("/enrollment/time.htm"sv);
inline constexpr auto restore_path("/enrollment/initiatePaperRestore.htm"sv);
inline constexpr auto restore_validate_path("/enrollment/validatePaperRestore.htm"sv);


}