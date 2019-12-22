#pragma once


#include<utility>
#include<type_traits>
#include<memory>
#include<system_error>
#include<exception>
#include<stdexcept>

#include<string>
#include<bitset>
#include<algorithm>
#include<cstring>
#include<cstdint>

#include<bitset>

#include"fast_io_impl/concept.h"
#include"fast_io_impl/eof.h"
#include"fast_io_impl/seek.h"
#include"fast_io_impl/precondition.h"
#include"fast_io_impl/mode.h"
#include"fast_io_impl/shared_base_table.h"
#include"fast_io_impl/base.h"
#include"fast_io_impl/fill_nc.h"
#include"fast_io_impl/transmit.h"
#include"fast_io_impl/stringbuf.h"
#include"fast_io_impl/rd_type.h"
#include"fast_io_impl/manip.h"
#include"fast_io_impl/read_write.h"
#include"fast_io_impl/ucs.h"
#include"fast_io_impl/text.h"
#include"fast_io_impl/handlers/native.h"
#include"fast_io_impl/iobuf.h"
#include"fast_io_impl/iomutex.h"
#include"fast_io_impl/wrapper.h"
#include"fast_io_impl/tie.h"
#include"fast_io_impl/flush.h"
//#include"fast_io_impl/dynamic.h"
#include"fast_io_impl/sync.h"
#include"fast_io_impl/streambuf_view.h"
#include"fast_io_impl/stream_view.h"
#include"fast_io_impl/ospan.h"

namespace fast_io
{

using pipe = immediately_flush<nobuf_reader<io_wrapper<system_pipe>>>;

using isystem_file = input_file_wrapper<system_file>;
using osystem_file = output_file_wrapper<system_file>;
using iosystem_file = io_file_wrapper<system_file>;


using sync = basic_sync<basic_file_wrapper<system_file,fast_io::open::app|fast_io::open::binary>>;
using fsync = basic_fsync<basic_file_wrapper<system_file,fast_io::open::app|fast_io::open::binary>>;

using sync_mutex = basic_iomutex<sync>;
using fsync_mutex = basic_iomutex<fsync>;

using ibuf = basic_ibuf<isystem_file>;
using obuf = basic_obuf<osystem_file>;
using iobuf = basic_iobuf<iosystem_file>;

static_assert(buffer_output_stream<obuf>,"obuf is not a buffer output stream");


using ibuf_mutex = basic_iomutex<ibuf>;
using obuf_mutex = basic_iomutex<obuf>;
using iobuf_mutex = basic_iomutex<iobuf>;
using istring_view_mutex = basic_iomutex<istring_view>;
using ostring_mutex = basic_iomutex<ostring>;
//using ibuf_dynamic = basic_ibuf<dynamic_input_stream>;
//using obuf_dynamic = basic_obuf<dynamic_output_stream>;
//using iobuf_dynamic = basic_iobuf<dynamic_io_stream>;
}

#ifndef FAST_IO_NO_IO_DEFAULT_DEVICES
#ifdef FAST_IO_IOSTREAM_AS_IO_HANDLE
#include"fast_io_impl/handlers/iostreams.h"
#elif FAST_IO_CSTDIO_AS_IO_HANDLE
#include"fast_io_impl/handlers/c_style.h"

namespace fast_io
{
inline c_style_io_handle out(stdout);
inline tie<c_style_io_handle,decltype(out)> in(out,stdin);
inline tie<immediately_flush<decltype(out)>,decltype(out)> err(out,stderr);
inline fast_io::basic_obuf<c_style_io_handle> log(stderr);
}
#else
namespace fast_io
{
inline basic_obuf<system_io_handle> out(native_stdout_number);
inline tie<basic_ibuf<system_io_handle>,decltype(out)> in(out,native_stdin_number);
inline tie<immediately_flush<system_io_handle>,decltype(out)> err(out,native_stderr_number);
inline basic_obuf<system_io_handle> log(native_stderr_number);
}
#endif
#endif

#include"fast_io_impl/network/network.h"
#include"fast_io_impl/crypto/crypto.h"
//#include"fast_io_impl/natural.h"
//#include"fast_io_impl/unsigned_extension.h"