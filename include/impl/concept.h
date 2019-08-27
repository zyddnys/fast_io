#pragma once
#include<utility>
#include<limits>
#include<type_traits>
#include"before_cpp20_concept.h"
#include"seek.h"

namespace fast_io
{
template<typename T>
concept bool input_stream()
{
	return requires(T in)
	{
		typename T::char_type;
		{in.read};
	};
};
template<typename T>
concept bool standard_input_stream()
{
	return input_stream<T>()&&requires(T in)
	{
		{in.get()}->typename T::char_type;
		{in.try_get()}->std::pair<typename T::char_type,bool>;
		{in.eof()}->bool;
	};
};

template<typename T>
concept bool mutex_stream()
{
	return requires(T t)
	{
		{t.mutex()};
	};
};

template<typename T>
concept bool mutex_input_stream = standard_input_stream<T>()&&mutex_stream<T>();

template<typename T>
concept bool output_stream()
{
	return requires(T out)
	{
		typename T::char_type;
		{out.write};
		{out.flush()};
	};
};

template<typename T>
concept bool io_stream = input_stream<T>()&&output_stream<T>();

template<typename T>
concept bool stream = input_stream<T>()||output_stream<T>();

template<typename T>
concept bool standard_output_stream()
{
	return output_stream<T>()&&requires(T out)
	{
		{out.put};
	};
};

template<typename T>
concept bool standard_io_stream = input_stream<T>()&&output_stream<T>()&&io_stream<T>;

template<typename T>
concept bool mutex_output_stream = standard_output_stream<T>()&&mutex_stream<T>();

template<typename T>
concept bool mutex_io_stream = input_stream<T>()&&output_stream<T>()&&io_stream<T>;

template<typename T>
concept bool random_access_stream()
{
	return stream<T>&&requires(T t)
	{
		{t.seek};
	};
};

/*	template<typename T>
	concept bool random_access_input_stream()
	{
		return input_stream<T>()&&requires(T in)
		{
			{in.seek}
		};
	};*/

}