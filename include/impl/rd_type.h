#pragma once

namespace fast_io
{
namespace details
{

template<std::integral T>
inline constexpr bool isspace(T ch)
{
	if(ch==20)
		return true;
	std::make_unsigned_t<T> e(ch);
	e-=9;
	return e<5;
}

}

template<standard_input_stream input>
inline constexpr auto eat_space_get(input& in)
{
	decltype(in.get()) ch(in.get());
	for(;details::isspace(ch);ch=in.get());
	return ch;
}

template<standard_input_stream input>
inline constexpr auto try_eat_space_get(input& in)
{
	auto ch(in.try_get());
	for(;details::isspace(ch.first);ch=in.try_get());
	return ch;
}

template<standard_input_stream input,std::integral T>
requires std::same_as<T,bool>
inline constexpr void scan(input& in, T& b)
{
	auto value(eat_space_get(in));
	if(value=='0')
		b=0;
	else
		b=1;
}

template<standard_output_stream output,std::integral T>
requires std::same_as<T,bool>
inline constexpr void print(output& out, T const& b)
{
	out.put(b+'0');
}

template<output_stream output>
inline void print(output& out,std::basic_string_view<typename output::char_type> str)
{
	out.write(str.cbegin(),str.cend());
}

template<output_stream output>
inline void print(output& out,std::exception const &e)
{
	print(out,e.what());
}

template<output_stream output>
inline void print(output& out,std::system_error const &e)
{
	auto const& code(e.code());
	print(out,"std::system_error, value:",code.value(),"\tmessage:",code.message());
}

template<standard_input_stream input>
inline void scan(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	str.push_back(eat_space_get(in));
	for(decltype(in.try_get()) ch;!(ch=in.try_get()).second&&!details::isspace(ch.first);str.push_back(ch.first));
}

template<standard_input_stream input>
inline void getline(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	for(decltype(in.try_get()) ch;!(ch=in.try_get()).second&&ch.first!='\n';str.push_back(ch.first));
}

template<standard_input_stream input>
inline void getwhole(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	for(decltype(in.try_get()) ch;!(ch=in.try_get()).second;str.push_back(ch.first));
}

namespace details
{
template<output_stream os,typename ...Args>
inline void fprint(os &out,std::basic_string_view<typename os::char_type> format)
{
	std::size_t percent_pos;
	for(;(percent_pos=format.find('%'))!=std::string_view::npos&&percent_pos+1!=format.size()&&format[percent_pos+1]=='%';format.remove_prefix(percent_pos+2))
		out.write(format.cbegin(),format.cbegin()+percent_pos+1);
	if(percent_pos!=std::string_view::npos)
		throw std::runtime_error("fprint() format error");
	out.write(format.cbegin(),format.cend());
}

template<output_stream os,typename T,typename ...Args>
inline void fprint(os &out,std::basic_string_view<typename os::char_type> format,T&& cr,Args&& ...args)
{
	std::size_t percent_pos;
	for(;(percent_pos=format.find('%'))!=std::string_view::npos&&percent_pos+1!=format.size()&&format[percent_pos+1]=='%';format.remove_prefix(percent_pos+2))
		out.write(format.cbegin(),format.cbegin()+percent_pos+1);
	if(percent_pos==std::string_view::npos)
	{
		out.write(format.cbegin(),format.cend());
		return;
	}
	else
	{
		out.write(format.cbegin(),format.cbegin()+percent_pos);
		format.remove_prefix(percent_pos+1);
	}
	print(out,std::forward<T>(cr));
	fprint(out,format,std::forward<Args>(args)...);
}
}

template<output_stream os,typename ...Args>
inline constexpr void fprint(os &out,std::basic_string_view<typename os::char_type> format,Args&& ...args)
{
	details::fprint(out,format,std::forward<Args>(args)...);
}

template<input_stream input>
inline constexpr void scan(input&){}

template<output_stream output>
inline constexpr void print(output&){}

template<input_stream input,typename T,typename R,typename ...Args>
inline constexpr void scan(input &in,T&& ref,R&& ref1,Args&& ...args)
{
	scan(in,std::forward<T>(ref));
	scan(in,std::forward<R>(ref1));
	scan(in,std::forward<Args>(args)...);
}

template<output_stream output,typename T,typename R,typename ...Args>
inline constexpr void print(output &out,T&&cr,R&&cr1,Args&& ...args)
{
	print(out,std::forward<T>(cr));
	print(out,std::forward<R>(cr1));
	print(out,std::forward<Args>(args)...);
}

template<output_stream output,typename ...Args>
inline constexpr void println(output &out,Args&& ...args)
{
	print(out,std::forward<Args>(args)...);
	out.put('\n');
}

template<output_stream output,typename ...Args>
inline constexpr void print_flush(output &out,Args&& ...args)
{
	print(out,std::forward<Args>(args)...);
	out.flush();
}

template<output_stream output,typename ...Args>
inline constexpr void println_flush(output &out,Args&& ...args)
{
	println(out,std::forward<Args>(args)...);
	out.flush();
}

template<output_stream output,typename ...Args>
inline constexpr void fprint_flush(output &out,Args&& ...args)
{
	fprint(out,std::forward<Args>(args)...);
	out.flush();
}

template<output_stream output,typename ...Args>
inline constexpr void write_flush(output& out,Args&& ...args)
{
	write(out,std::forward<Args>(args)...);
	out.flush();
}

}