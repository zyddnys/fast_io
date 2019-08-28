#pragma once
#include"impl/concept.h"
#include"impl/precondition.h"
#include"impl/mode.h"
#include"impl/handlers/c_style.h"
#include"impl/mode.h"
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
#include"impl/flush.h"
#include"impl/erasure.h"
#include"impl/dynamic.h"
#include"impl/sync.h"

namespace fast_io
{
using system_io_handle = posix_io_handle;
using system_file = posix_file;
using system_ohandle = ierasure<system_io_handle>;
using system_ihandle = oerasure<system_io_handle>;

struct system_io_collections
{
basic_obuf<system_ohandle> out;
tie<basic_ibuf<posix_io_handle>,decltype(out)> in;
tie<immediately_flush<decltype(out)>,decltype(out)> err;
system_io_collections():out(1),in(out,0),err(out,2){}
};

using isystem_file = input_wrapper<system_file>;
using osystem_file = output_wrapper<system_file>;
using iosystem_file = io_wrapper<system_file>;

using osync = basic_sync<basic_wrapper<system_file,fast_io::open::out|fast_io::open::app>>;
using ofsync = basic_fsync<basic_wrapper<system_file,fast_io::open::out|fast_io::open::app>>;

using ibuf = basic_ibuf<isystem_file>;
using obuf = basic_obuf<osystem_file>;
using iobuf = basic_iobuf<iosystem_file>;

using istring_view = basic_istring_view<std::string_view>;
using ostring = basic_ostring<std::string>;


using osync_mutex = basic_iomutex<osync>;
using ofsync_mutex = basic_iomutex<ofsync>;

using ibuf_mutex = basic_iomutex<ibuf>;
using obuf_mutex = basic_iomutex<obuf>;
using iobuf_mutex = basic_iomutex<iobuf>;
using istring_view_mutex = basic_iomutex<istring_view>;
using ostring_mutex = basic_iomutex<ostring>;

using c_style_ohandle = ierasure<c_style_io_handle>;
using c_style_ihandle = oerasure<c_style_io_handle>;
inline c_style_ohandle out(stdout);
inline tie<c_style_ihandle,decltype(out)> in(out,stdin);
inline tie<immediately_flush<decltype(out)>,decltype(out)> err(out,stderr);


using ibuf_dynamic = basic_ibuf<dynamic_input_stream>;
using obuf_dynamic = basic_obuf<dynamic_output_stream>;
using iobuf_dynamic = basic_iobuf<dynamic_io_stream>;
}

#include"impl/base.h"
#include"impl/rd_type.h"
#include"impl/manip.h"
#include"impl/read_write.h"
//#include"impl/unicode.h"