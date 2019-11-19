#pragma once

#include"fast_io_hosted.h"
//This header file defines in/out/err/log like traditional C/C++ does

namespace fast_io
{
using out_type = basic_obuf<system_io_handle>;
using in_type = tie<basic_ibuf<system_io_handle>,out_type>;
using err_type = tie<system_io_handle,out_type>;
using log_type = out_type;

inline out_type out(native_stdout_number);
inline in_type in(out,native_stdin_number);
inline err_type err(out,native_stderr_number);
inline log_type log(native_stderr_number);
}
