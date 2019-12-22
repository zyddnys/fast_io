#include<cstdlib>
#include<new>
#include<exception>

#include<windows.h>
#undef interface
#undef min
#undef max


[[nodiscard]] inline void* operator new(std::size_t bytes) noexcept
{
	auto allocated(HeapAlloc(GetProcessHeap(),0,bytes));
	if(allocated==nullptr) [[unlikely]]
		std::terminate();
	return allocated;
}

[[nodiscard]] inline void* operator new(std::size_t bytes,std::nothrow_t const&) noexcept
{
	return operator new(bytes);
}

[[nodiscard]] inline void* operator new[](std::size_t bytes) noexcept
{
	return operator new[](bytes);
}

inline void operator delete(void* ptr) noexcept
{
	if(ptr)
	{
		if(!HeapFree(GetProcessHeap(),0,ptr))
		{
			std::terminate();
		}
	}
}

inline void operator delete(void* ptr,std::size_t) noexcept
{
	::operator delete(ptr);
}

inline void operator delete[](void* ptr) noexcept
{
	::operator delete(ptr);
}


#include"timer.h"

int main()
{

	constexpr std::size_t bytes(10);
	constexpr std::size_t loops(10000000);
	std::size_t sm(0);
	{
	cqw::timer tm("HeapAlloc/HeapFree");
	auto hp(GetProcessHeap());
	for(std::size_t i(0);i!=loops;++i)
	{
		auto v(HeapAlloc(GetProcessHeap(),0,bytes));
		sm+=fast_io::bit_cast<std::size_t>(v);
		HeapFree(GetProcessHeap(),0,v);
	}
	}
	{
	cqw::timer tm("new/delete");
	for(std::size_t i(0);i!=loops;++i)
	{
		auto v(new char[bytes]);
//		sm+=fast_io::bit_cast<std::size_t>(v);
		delete[] v;
	}
	}

	println(fast_io::out,sm);
	return 0;
}