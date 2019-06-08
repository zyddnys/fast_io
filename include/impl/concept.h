#ifndef FAST_IO_CONCEPT_H
#define FAST_IO_CONCEPT_H

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
			typename T::char_type;
			typename T::int_type;
			{in.get()};
			{in.eof()}->bool;
			{in}->bool;
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
	concept bool mutex_input_stream()
	{
		return standard_input_stream<T>()&&mutex_stream<T>();
	};

	
	template<typename T>
	concept bool output_stream()
	{
		return requires(T out)
		{
			{out.write};
			{out.flush()}->void;
		};
	};
	template<typename T>
	concept bool standard_output_stream()
	{
		return output_stream<T>()&&requires(T out)
		{
			typename T::char_type;
			typename T::int_type;
			{out.put};
		};
	};

	template<typename T>
	concept bool mutex_output_stream()
	{
		return standard_output_stream<T>()&&mutex_stream<T>();
	};

	template<typename T>
	concept bool Integral = std::numeric_limits<T>::is_integer;
	
	template<typename T>
	concept bool Signed_integer=Integral<T>&&
						std::numeric_limits<T>::is_signed;
	
	template<typename T>
	concept bool Unsigned_integer=Integral<T>&&
						!std::numeric_limits<T>::is_signed;
	
	template<typename T>
	concept bool Pointer = std::is_pointer<T>::value;
}
#endif