#pragma once

#include "otp.h"
#include "ecb.h"
#include "cbc.h"
#include "ctr.h"
#include "speck.h"
//#include "aes.h"

namespace fast_io::crypto
{

template<input_stream in>
inline ictr_speck=basic_ictr<in,speck::speck_enc_128_256>;

template<output_stream out>
inline octr_speck=basic_octr<out,speck::speck_enc_128_256>;

template<input_stream in>
inline icbc_speck=basic_ictr<in,speck::speck_dec_128_256>;

template<output_stream out>
inline ocbc_speck=basic_octr<out,speck::speck_enc_128_256>;

}