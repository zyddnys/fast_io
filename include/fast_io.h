#pragma once
#include"impl/concept.h"
#include"impl/precondition.h"
#include"impl/mode.h"
#include"impl/handlers/c_style.h"
#include"impl/mode.h"
#include"impl/base.h"
#include"impl/rd_type.h"
#include"impl/manip.h"
#include"impl/read_write.h"
#include"impl/ucs.h"
#include"impl/text.h"
#ifdef _WIN32_WINNT
#include"impl/handlers/win32.h"
#endif
#include"impl/handlers/posix.h"	
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
#ifdef _WIN32_WINNT
using system_file = win32_file;
#else
using system_file = posix_file;
#endif

using iposix_pipe = nobuf_reader<input_wrapper<posix_pipe>>;
using oposix_pipe = immediately_flush<nobuf_reader<output_wrapper<posix_pipe>>>;
using ioposix_pipe = immediately_flush<nobuf_reader<io_wrapper<posix_pipe>>>;

using system_io_handle = posix_io_handle;
using system_ohandle = ierasure<system_io_handle>;
using system_ihandle = oerasure<system_io_handle>;

struct system_io_collections
{
basic_obuf<system_ohandle> out;
tie<basic_ibuf<posix_io_handle>,decltype(out)> in;
tie<immediately_flush<decltype(out)>,decltype(out)> err;
system_io_collections():out(1),in(out,0),err(out,2){}
};

using isystem_file = input_file_wrapper<system_file>;
using osystem_file = output_file_wrapper<system_file>;
using iosystem_file = io_file_wrapper<system_file>;



using sync = basic_sync<basic_file_wrapper<system_file,fast_io::open::app|fast_io::open::binary>>;
using osync = ierasure<sync>;
using fsync = basic_fsync<basic_file_wrapper<system_file,fast_io::open::app|fast_io::open::binary>>;
using ofsync = ierasure<fsync>;

using sync_mutex = basic_iomutex<sync>;
using osync_mutex = basic_iomutex<osync>;
using fsync_mutex = basic_iomutex<fsync>;
using ofsync_mutex = basic_iomutex<ofsync>;

using ibuf = basic_ibuf<isystem_file>;
using obuf = basic_obuf<osystem_file>;
using iobuf = basic_iobuf<iosystem_file>;

using istring_view = basic_istring_view<std::string_view>;
using ostring = basic_ostring<std::string>;

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
