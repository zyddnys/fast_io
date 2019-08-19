#pragma once
#include"concept.h"

namespace fast_io
{
inline input_stream& read(input_stream& in,Integral& v)
{
	in.read(&v,&v+1);
	return in;
}

inline output_stream& write(output_stream& out,Integral const& v)
{
	out.write(&v,&v+1);
	return out;
}


}