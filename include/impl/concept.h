#pragma once
#include<utility>
#include<limits>
#include<type_traits>
#include"before_cpp20_concept.h"
#include"seek.h"

namespace fast_io
{
	
namespace details
{	
template<typename T>
concept bool input_stream_impl()
{
	return requires(T in)
	{
		typename T::char_type;
		{in.read};
	};
};

template<typename T>
concept bool output_stream_impl()
{
	return requires(T out)
	{
		typename T::char_type;
		{out.write};
		{out.flush()};
	};
};

template<typename T>
concept bool mutex_stream_impl()
{
	return requires(T t)
	{
		{t.mutex()};
	};
};

template<typename T>
concept bool standard_input_stream_impl()
{
	return requires(T in)
	{
		{in.get()}->typename T::char_type;
		{in.try_get()}->std::pair<typename T::char_type,bool>;
		{in.eof()}->bool;
	};
}

template<typename T>
concept bool standard_output_stream_impl()
{
	return requires(T out)
	{
		{out.put};
	};
};

template<typename T>
concept bool random_access_stream_impl()
{
	return requires(T t)
	{
		{t.seek};
	};
};

}

template<typename T>
concept bool stream = details::input_stream_impl<T>()||details::output_stream_impl<T>();

template<typename T>
concept bool mutex_stream = stream<T>&&details::mutex_stream_impl<T>();

template<typename T>
concept bool input_stream = stream<T>&&details::input_stream_impl<T>();

template<typename T>
concept bool mutex_input_stream = input_stream<T>&&mutex_stream<T>;

template<typename T>
concept bool output_stream = stream<T>&&details::output_stream_impl<T>();

template<typename T>
concept bool mutex_output_stream = output_stream<T>&&mutex_stream<T>;

template<typename T>
concept bool random_access_stream = stream<T>&&details::random_access_stream_impl<T>();

template<typename T>
concept bool io_stream = input_stream<T>&&output_stream<T>;

template<typename T>
concept bool mutex_io_stream = mutex_input_stream<T>&&mutex_output_stream<T>;

template<typename T>
concept bool standard_input_stream = input_stream<T>&&details::standard_input_stream_impl<T>();

template<typename T>
concept bool standard_output_stream = output_stream<T>&&details::standard_output_stream_impl<T>();

template<typename T>
concept bool standard_io_stream = standard_input_stream<T>&&standard_output_stream<T>;

}