#pragma once

namespace fast_io
{

template<input_stream in>
requires output_stream<typename in::native_handle_type>
class io_device
{
	in ih;
public:
	using native_handle_type = typename in::native_handle_type;
	using char_type = typename native_handle_type::char_type;	

};

}