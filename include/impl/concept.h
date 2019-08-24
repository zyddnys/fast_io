#pragma once

#include<limits>
#include<type_traits>
namespace fast_io
{
	template<typename T>
	concept bool input_stream()
	{
		return requires(T in)
		{
			{in.read};
		};
	};
	template<typename T>
	concept bool standard_input_stream()
	{
		return input_stream<T>()&&requires(T in)
		{
			T::traits_type;
			{in.get()}->typename T::traits_type::int_type;
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
			{out.write};
			{out.flush()};
		};
	};

	template<typename T>
	concept bool io_stream = input_stream<T>()&&output_stream<T>();

	template<typename T>
	concept bool standard_output_stream()
	{
		return output_stream<T>()&&requires(T out)
		{
			T::traits_type;
			{out.put(T::traits_type::int_type)};
		};
	};

	template<typename T>
	concept bool standard_io_stream = input_stream<T>()&&output_stream<T>()&&io_stream<T>;

	template<typename T>
	concept bool mutex_output_stream = standard_output_stream<T>()&&mutex_stream<T>();

	template<typename T>
	concept bool mutex_io_stream = input_stream<T>()&&output_stream<T>()&&io_stream<T>;

/*	template<typename T>
	concept bool random_access_input_stream()
	{
		return input_stream<T>()&&requires(T in)
		{
			{in.seek}
		};
	};*/


	template<typename T>
	concept bool Integral = std::numeric_limits<T>::is_integer;

	template<typename T>
	concept bool Signed_integer=Integral<T>&&std::numeric_limits<T>::is_signed;

	template<typename T>
	concept bool Unsigned_integer=Integral<T>&&!Signed_integer<T>;
}