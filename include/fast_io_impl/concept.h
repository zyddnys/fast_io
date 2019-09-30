#pragma once

#include"before_cpp20_concept.h"

namespace fast_io
{
	
namespace details
{	
template<typename T>
concept input_stream_impl = requires(T in)
{
	typename T::char_type;
	{in.read};
};

template<typename T>
concept output_stream_impl = requires(T out)
{
	typename T::char_type;
	{out.write};
	{out.flush()};
};

template<typename T>
concept mutex_stream_impl = requires(T t)
{
	{t.mutex()};
};

template<typename T>
concept standard_input_stream_impl = requires(T in)
{
	{in.get()}->typename T::char_type;
	{in.try_get()}->std::pair<typename T::char_type,bool>;
};

template<typename T>
concept standard_output_stream_impl = requires(T out)
{
	{out.put};
};

template<typename T>
concept random_access_stream_impl = requires(T t)
{
	{t.seek};
};

}

template<typename T>
concept stream = details::input_stream_impl<T>||details::output_stream_impl<T>;

template<typename T>
concept mutex_stream = stream<T>&&details::mutex_stream_impl<T>;

template<typename T>
concept mutex_input_stream = mutex_stream<T>&&details::input_stream_impl<T>;

template<typename T>
concept mutex_output_stream = mutex_stream<T>&&details::output_stream_impl<T>;

template<typename T>
concept input_stream = stream<T>&&details::input_stream_impl<T>&&!mutex_input_stream<T>;

template<typename T>
concept output_stream = stream<T>&&details::output_stream_impl<T>&&!mutex_output_stream<T>;

template<typename T>
concept random_access_stream = stream<T>&&details::random_access_stream_impl<T>;

template<typename T>
concept io_stream = input_stream<T>&&output_stream<T>;

template<typename T>
concept standard_input_stream = input_stream<T>&&details::standard_input_stream_impl<T>;

template<typename T>
concept standard_output_stream = output_stream<T>&&details::standard_output_stream_impl<T>;

template<typename T>
concept standard_io_stream = standard_input_stream<T>&&standard_output_stream<T>;
template<typename T>
concept mutex_io_stream = mutex_input_stream<T>&&mutex_output_stream<T>;

}