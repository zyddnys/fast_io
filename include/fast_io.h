#pragma once
#include"impl/concept.h"
#include"impl/mode.h"
#include"impl/handlers/c_style.h"
#ifdef _WIN32_WINNT
#endif
//#ifdef _POSIX_C_SOURCE
#include"impl/handlers/posix.h"	
//#endif
#include"impl/stringbuf.h"
#include"impl/iobuf.h"
#include"impl/iomutex.h"
#include"impl/wrapper.h"
#include"impl/tie.h"

namespace fast_io
{
using system_io_handle = posix_io_handle;
using system_file = posix_file;
inline basic_obuf<posix_io_handle> system_out(1);
inline tie<basic_ibuf<posix_io_handle>,basic_obuf<posix_io_handle>> system_in(system_out,0);
inline tie<basic_obuf<posix_io_handle>,basic_obuf<posix_io_handle>> system_err(system_out,2);

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

inline c_style_io_handle out(stdout);
inline tie<c_style_io_handle,c_style_io_handle> in(out,stdin);
inline tie<c_style_io_handle,c_style_io_handle> err(out,stderr);

}

#include"impl/rd_type.h"
#include"impl/manip.h"
#include"impl/read_write.h"
#include"impl/unicode.h"