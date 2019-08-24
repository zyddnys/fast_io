#pragma once
#include"concept.h"
#include"mode.h"

namespace fast_io
{

template<input_stream T>
class input_wrapper:public T
{
public:
	using traits_type = typename T::traits_type;
	using native_handle_type = typename T::native_handle_type;
	template<typename ...Args>
	constexpr input_wrapper(native_interface_t,Args&& ...args):T(std::forward<Args>(args)...){}
	template<std::size_t om>
	constexpr input_wrapper(std::string_view file,open::interface_t<om>):T(file,open::interface<fast_io::open::mode(om)|fast_io::open::in>){}
	constexpr input_wrapper(std::string_view file):T(file,open::interface<fast_io::open::in>){}
	constexpr input_wrapper(std::string_view file,open::mode const& m):T(file,m|fast_io::open::in){}
	constexpr input_wrapper(std::string_view file,std::string_view mode):input_wrapper(file,fast_io::open::c_style(mode)){}
};

template<output_stream T>
class output_wrapper:public T
{
public:
	using traits_type = typename T::traits_type;
	using native_handle_type = typename T::native_handle_type;
	template<typename ...Args>
	constexpr output_wrapper(native_interface_t,Args&& ...args):T(std::forward<Args>(args)...){}
	template<std::size_t om>
	constexpr output_wrapper(std::string_view file,open::interface_t<om>):T(file,open::interface<fast_io::open::mode(om)|fast_io::open::out>){}
	constexpr output_wrapper(std::string_view file):T(file,open::interface<fast_io::open::out>){}
	constexpr output_wrapper(std::string_view file,open::mode const& m):T(file,m|fast_io::open::out){}
	constexpr output_wrapper(std::string_view file,std::string_view mode):output_wrapper(file,fast_io::open::c_style(mode)){}	
};

template<io_stream T>
class io_wrapper:public T
{
public:
	using traits_type = typename T::traits_type;
	using native_handle_type = typename T::native_handle_type;
	template<typename ...Args>
	constexpr io_wrapper(native_interface_t,Args&& ...args):T(std::forward<Args>(args)...){}
	template<std::size_t om>
	constexpr io_wrapper(std::string_view file,open::interface_t<om>):T(file,open::interface<fast_io::open::mode(om)|fast_io::open::in|fast_io::open::out>){}
	constexpr io_wrapper(std::string_view file):T(file,open::interface<fast_io::open::in|fast_io::open::out>){}
	constexpr io_wrapper(std::string_view file,open::mode const& m):T(file,m|fast_io::open::in|fast_io::open::out){}
	constexpr io_wrapper(std::string_view file,std::string_view mode):io_wrapper(file,fast_io::open::c_style(mode)){}
};

}