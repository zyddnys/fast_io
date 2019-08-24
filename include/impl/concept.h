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
			typename T::traits_type;
			{in.read};
		};
	};
	template<typename T>
	concept bool standard_input_stream()
	{
		return input_stream<T>()&&requires(T in)
		{
			{in.get()};
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
			typename T::traits_type;
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
			typename T::traits_type;
			{out.put};
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
// without std support it is a crap. Will use standard libraries after C++ 20 being official published. Do not use these concepts!!!

	template<typename T>
	concept bool Integral = std::numeric_limits<T>::is_integer;

	template<typename T>
	concept bool Signed_integer=Integral<T>&&std::numeric_limits<T>::is_signed;

	template<typename T>
	concept bool Unsigned_integer=Integral<T>&&!Signed_integer<T>;

	template<typename T>
	concept bool Trivial_copyable=std::is_trivially_copyable_v<T>;

	template<typename Cont>
	concept bool Container()
	{
		return requires(Cont c)
		{
			{begin(c)};
			{end(c)};
		};
	};

	template<typename Cont>
	concept bool Dynamic_size_container()
	{
		return Container<Cont>() && requires(Cont c)
		{
			{c.resize};
			{size(c)};
		};
	};

	template<typename Cont>
	concept bool Contiguous_container()
	{
		return Container<Cont>() && requires(Cont c)
		{
			{data(c)};
		};
	};

	template<typename Cont>
	concept bool Trivial_copyable_container = Container<Cont>() && Trivial_copyable<typename Cont::value_type>;

	template<typename Cont>
	concept bool Contiguous_trivial_container = Contiguous_container<Cont>() && Trivial_copyable_container<Cont>;

	template<typename Cont>
	concept bool Contiguous_dynamic_size_container = Dynamic_size_container<Cont>()&&Contiguous_container<Cont>();

	template<typename Cont>
	concept bool Contiguous_trivial_dynamic_size_container = Contiguous_trivial_container<Cont>&&Contiguous_dynamic_size_container<Cont>;

	template<typename Cont>
	concept bool Contiguous_fixed_size_container = Contiguous_container<Cont>()&&!Dynamic_size_container<Cont>();

	template<typename Cont>
	concept bool Contiguous_fixed_size_trivial_copyable_container = Contiguous_fixed_size_container<Cont>&& Trivial_copyable_container<Cont>;

	template<typename Cont>
	concept bool Contiguous_fixed_size_none_trivial_copyable_container =Contiguous_fixed_size_container<Cont>&&!Contiguous_fixed_size_trivial_copyable_container<Cont>;

}