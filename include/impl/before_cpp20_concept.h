#pragma once
// Since the library is written before C++20. Will use standard libraries concepts after C++ 20 being official published. PLEASE Do not use these concepts!!!
#include<type_traits>
#ifdef __cpp_lib_concepts
#include<concepts>
#else
#include"ported/concepts"		//use my migrated version of concepts
#endif

namespace fast_io
{

template<typename T>
concept Trivial_copyable=std::is_trivially_copyable_v<T>;

template<typename Cont>
concept Container = requires(Cont c)
{
	{begin(c)};
	{end(c)};
};

template<typename Cont>
concept Dynamic_size_container = 
Container<Cont> && requires(Cont c)
{
	{c.insert};
	{size(c)};
};

template<typename Cont>
concept Contiguous_container = Container<Cont> && requires(Cont c)
{
	{data(c)};
};

template<typename Cont>
concept Trivial_copyable_container = Container<Cont> && Trivial_copyable<typename Cont::value_type>;

template<typename Cont>
concept Contiguous_trivial_container = Contiguous_container<Cont> && Trivial_copyable_container<Cont>;

template<typename Cont>
concept Contiguous_dynamic_size_container = Dynamic_size_container<Cont>&&Contiguous_container<Cont>;

template<typename Cont>
concept Contiguous_trivial_dynamic_size_container = Contiguous_trivial_container<Cont>&&Contiguous_dynamic_size_container<Cont>;

template<typename Cont>
concept Contiguous_fixed_size_container = Contiguous_container<Cont>&&!Dynamic_size_container<Cont>;

template<typename Cont>
concept Contiguous_fixed_size_trivial_copyable_container = Contiguous_fixed_size_container<Cont>&& Trivial_copyable_container<Cont>;

template<typename Cont>
concept Contiguous_fixed_size_none_trivial_copyable_container =Contiguous_fixed_size_container<Cont>&&!Contiguous_fixed_size_trivial_copyable_container<Cont>;

}