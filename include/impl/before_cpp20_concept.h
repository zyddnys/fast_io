#pragma once
// Since the library is written before C++20. Will use standard libraries concepts after C++ 20 being official published. PLEASE Do not use these concepts!!!

namespace fast_io
{

template<typename T>
concept bool Integral = std::numeric_limits<T>::is_integer;

template<typename T>
concept bool Signed_integer=Integral<T>&&std::numeric_limits<T>::is_signed;

template<typename T>
concept bool Unsigned_integer=Integral<T>&&!Signed_integer<T>;

template<typename T>
concept bool Floating_point = std::numeric_limits<T>::is_iec559;

template<typename T>
concept bool Trivial_copyable=std::is_trivially_copyable_v<T>;

template<typename T>
concept bool Integral = std::numeric_limits<T>::is_integer;

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
		{c.insert};
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