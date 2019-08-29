#pragma once

#include"ntkernel_category.hpp"
#include<windows.h>
#include<ntifs.h>
#include"../mode.h"
#include<memory>

namespace fast_io
{

namespace detail
{
	
}

class nt_file
{
	HANDLE handle;
	void closeimpl() noexcept
	{
		if(handle)
			NtClose(handle);
	}
public:
	using char_type = char;
	using native_handle_type = HANDLE;
	template<typename ...Args>
	nt_file(native_interface_t,Args&& ...args)
	{
		auto status(NtCreateFile(std::addressof(handle),std::forward<Args>(args)...));
		if(status)
			throw std::system_error(status,ntkernel_error_category::ntkernel_category());
	}
	template<openmode om>
	nt_file(std::string_view filename,open_interface_t<om>)
	{
//https://stackoverflow.com/questions/7430959/how-to-make-createfile-as-fast-as-possible
		OBJECT_ATTRIBUTES Oa{};
		IO_STATUS_BLOCK IoSb{};
		UNICODE_STRING Name_U;
		RtlInitUnicodeString(std::addressof(Name_U),filename.data());
		Oa.Length = sizeof(Oa);
		Oa.ObjectName = std::addressof(Name_U);
		Oa.RootDirectory = 
	}
	nt_file(nt_file const&) = delete;
	nt_file& operator=(nt_file const&) = delete;
	nt_file(nt_file&& b) noexcept:handle(b.handle)
	{
		b.handle = nullptr;
	}
	nt_file& operator=(nt_file&& b) noexcept
	{
		if(std::addressof(b))
		{
			closeimpl();
			handle = b.handle;
			b.handle = nullptr;
		}
		return *this;
	}
	~nt_file()
	{
		closeimpl();
	}
};

}