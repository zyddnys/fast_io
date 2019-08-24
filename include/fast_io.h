#pragma once
#include"impl/mode.h"
#include"impl/handlers/c_style.h"
/*
#ifdef _WIN32_WINNT
#include"impl/handlers/windows.h"
#elif _POSIX_C_SOURCE
#include"impl/handlers/posix.h"	
#endif*/
#include"impl/concept.h"
#include"impl/rd_type.h"
#include"impl/read_write.h"
#include"impl/manip.h"
#include"impl/stringbuf.h"
#include"impl/iobuf.h"
#include"impl/iomutex.h"
#include"impl/wrapper.h"
#include"impl/tie.h"

namespace fast_io
{
/*	#ifdef _WIN32_WINNT
		using default_input = windows_file<open_mode::in>;
		using default_output = windows_file<open_mode::out>;
	#elif _POSIX_C_SOURCE
		using default_input = posix_file<open_mode::in>;
		using default_output = posix_file<open_mode::out>;
	#else
		using default_input = std_file<open_mode::in>;
		using default_output = std_file<open_mode::out>;
	#endif*/
	using system_file = c_style_file;
	using isystem_file = input_wrapper<system_file>;
	using osystem_file = output_wrapper<system_file>;
	using iosystem_file = io_wrapper<system_file>;

	using ibuf = basic_ibuf<isystem_file>;
	using obuf = basic_obuf<osystem_file>;
	using iobuf = basic_iobuf<iosystem_file>;

	using ibuf_string_view = basic_ibuf_string_view<std::string_view>;
	using obuf_string = basic_obuf_string<std::string>;
	using ibuf_mutex = basic_imutex<ibuf>;
	using obuf_mutex = basic_omutex<obuf>;
	using iobuf_mutex = basic_iomutex<iobuf>;
	using ibuf_string_view_mutex = basic_imutex<ibuf_string_view>;
	using obuf_string_mutex = basic_omutex<obuf_string>;
/*	using inbuf = basic_ibuf<std_in,char>;
	using outbuf = basic_obuf<std_out,char>;
	using inbuf_mutex = basic_imutex<inbuf>;
	using outbuf_mutex = basic_omutex<outbuf>;
	using log = basic_obuf<std_err,char>;
	using log_mutex = basic_omutex<log>;
	inline std_in in;//stdin
	inline std_out out;//stdout
	inline std_err err;//stderr*/
}