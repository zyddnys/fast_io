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

inline std::string normalize_serial(std::string_view serial)
{
	std::string ret;
	for(auto const& e : serial)
		if('a'<=e&&'e'<='z')
			ret.push_back(e-'a'+'A');
		else if(e!='-')
			ret.push_back(e);
	return ret;
}

inline std::string normalize_serial(key_info const& k)
{
	return normalize_serial(k.serial);
}

inline constexpr std::byte char_to_byte(char c)
{
	if (c >= '0' && c <= '9')
		return static_cast<std::byte>(c - '0');
	auto index(static_cast<unsigned char>(c + 10 - 65));
	if (c >= 'I') --index;
	if (c >= 'L') --index;
	if (c >= 'O') --index;
	if (c >= 'S') --index;
	return static_cast<std::byte>(index);
}
inline constexpr char byte_to_char(std::byte b)
{
	auto index(std::to_integer<unsigned char>(b) & 0x1f);
	if(index<10)
		return static_cast<char>(index+'0');
	index+=55;
	if (index >= 73) ++index;
	if (index >= 76) ++index;
	if (index >= 79) ++index;
	if (index >= 83) ++index;
	return static_cast<char>(index);
}

inline std::string restore_code(key_info const& k)
{
	fast_io::sha1 sha1;
	print(sha1,normalize_serial(k));
	send(sha1,k.secret_key.cbegin(),k.secret_key.cend());
	flush(sha1);
	if constexpr(std::endian::native==std::endian::little)
	{
		auto digest(sha1.digest);
		for(auto & e : digest)
			e=fast_io::details::big_endian(e);
		auto byte_digest(std::as_bytes(std::span{digest}).last(10));
		std::string result;
		for(auto const& e : byte_digest)
			result.push_back(byte_to_char(e));
		return result;
	}
	else
	{
		auto byte_digest(std::as_bytes(std::span{sha1.digest}).last(10));
		std::string result;
		for(auto const& e : byte_digest)
			result.push_back(byte_to_char(e));
		return result;
	}
}

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
		"s\nSerial:",ref.reference.serial,"\tKey:",fast_io::base_split<bas,uppercase>(ref.reference.secret_key,','),
		"\tRestore:",restore_code(ref.reference));
}

template<fast_io::buffer_output_stream output>
inline constexpr void print_define(output& out,key_info const& k)
{
	print_define(out,fast_io::dec(k));
}

}