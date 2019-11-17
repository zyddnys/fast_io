#pragma once
#include<chrono>

namespace battlenet
{

inline auto current_time()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

struct key_info
{
	std::chrono::milliseconds time_difference={};
	std::string serial;
	std::vector<std::uint8_t> secret_key;
};

inline auto to_time_difference(std::uint64_t t)
{
	return std::chrono::milliseconds(static_cast<std::int64_t>(t)-
		std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}

inline auto server_time(key_info const& info)
{
	return std::chrono::system_clock::now()+info.time_difference;
}

inline std::uint64_t current_time_stamp(key_info const& info)
{
	return std::chrono::duration_cast<std::chrono::seconds>(server_time(info).time_since_epoch()).count()/30;
}

template<fast_io::character_output_stream output>
inline constexpr void write_define(output& out,key_info const& k)
{
	write(out,k.time_difference);
	write(out,k.serial);
	write(out,k.secret_key);
}

template<fast_io::character_input_stream input>
inline constexpr void read_define(input& in,key_info& k)
{
	read(in,k.time_difference);
	read(in,k.serial);
	read(in,k.secret_key);
}

template<std::size_t bas,bool uppercase,fast_io::buffer_output_stream output,typename T>
requires std::same_as<key_info,std::remove_cvref_t<T>>
inline constexpr void print_define(output& out,fast_io::manip::base_t<bas,uppercase,T> ref)
{
	print(out,"Time difference:",ref.reference.time_difference,
		"s\tBlizzard Auth Server Time since epoch:",
		server_time(ref.reference),
		"s\nSerial:",ref.reference.serial,"\tKey:",fast_io::base_split<bas,uppercase>(ref.reference.secret_key,','));
}


template<fast_io::buffer_output_stream output>
inline constexpr void print_define(output& out,key_info const& k)
{
	print_define(out,fast_io::dec(k));
}

}