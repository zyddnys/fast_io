#pragma once
#include<exception>
#include<cstddef>
#include<system_error>
#include<array>
#include<utility>
#include<limits>
#include<type_traits>
#include<memory>
#include<stdexcept>
#include<string_view>
#include<string>
#include<bitset>
#include<algorithm>
#include"fast_io_impl/concept.h"
#include"fast_io_impl/eof.h"
#include"fast_io_impl/seek.h"
#include"fast_io_impl/precondition.h"
#include"fast_io_impl/mode.h"
#include"fast_io_impl/base.h"
#include"fast_io_impl/stringbuf.h"
#include"fast_io_impl/rd_type.h"
#include"fast_io_impl/manip.h"
#include"fast_io_impl/read_write.h"
#include"fast_io_impl/ucs.h"
#include"fast_io_impl/text.h"
#if defined(__WINNT__) || defined(_MSC_VER)
#include"fast_io_impl/handlers/win32.h"
#else
#include"fast_io_impl/handlers/posix.h"
#endif
#include"fast_io_impl/iobuf.h"
#include"fast_io_impl/iomutex.h"
#include"fast_io_impl/wrapper.h"
#include"fast_io_impl/tie.h"
#include"fast_io_impl/flush.h"
#include"fast_io_impl/erasure.h"
#include"fast_io_impl/dynamic.h"
#include"fast_io_impl/sync.h"
#include"fast_io_impl/streambuf_view.h"
#include"fast_io_impl/stream_view.h"

namespace fast_io
{

using isystem_pipe = nobuf_reader<input_wrapper<system_pipe>>;
using osystem_pipe = immediately_flush<nobuf_reader<output_wrapper<system_pipe>>>;
using iosystem_pipe = immediately_flush<nobuf_reader<io_wrapper<system_pipe>>>;

using system_ohandle = ierasure<system_io_handle>;
using system_ihandle = oerasure<system_io_handle>;

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
using ibuf_dynamic = basic_ibuf<dynamic_input_stream>;
using obuf_dynamic = basic_obuf<dynamic_output_stream>;
using iobuf_dynamic = basic_iobuf<dynamic_io_stream>;
}

#ifdef FAST_IO_IOSTREAM_AS_IO_HANDLE
#include"fast_io_impl/handlers/iostreams.h"
#elif FAST_IO_CSTDIO_AS_IO_HANDLE
#include"fast_io_impl/handlers/c_style.h"

namespace fast_io
{
using c_style_ohandle = ierasure<c_style_io_handle>;
using c_style_ihandle = oerasure<c_style_io_handle>;
inline c_style_ohandle out(stdout);
inline tie<c_style_ihandle,decltype(out)> in(out,stdin);
inline tie<immediately_flush<decltype(out)>,decltype(out)> err(out,stderr);
inline fast_io::basic_obuf<c_style_ohandle> log(stderr);
}
#else
namespace fast_io
{
inline basic_obuf<system_ohandle> out(native_stdout);
inline tie<basic_ibuf<system_io_handle>,decltype(out)> in(out,native_stdin);
inline tie<immediately_flush<system_ohandle>,decltype(out)> err(out,native_stderr);
inline basic_obuf<system_ohandle> log(native_stderr);
}
#endif

#include"fast_io_impl/network/network.h"
#include"fast_io_impl/crypto/crypto.h"
