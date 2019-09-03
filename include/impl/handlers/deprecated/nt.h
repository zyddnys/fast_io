//impossible to implement this directly

#pragma once

#include"nt_exception.h"
#include<windows.h>
#include<libloaderapi.h>
#include<winternl.h>

namespace fast_io
{

namespace nt_details
{
namespace
{
enum class rtl_path_type{unknown, unc_absolute,drive_absolute, drive_relative,rooted, relative, local_device, root_local_device};
/*
struct rtl_path_and_unknown
{
	rtl_path_type type;
	ULONG unknown;
};*/

inline auto const ntdll_module_handle(GetModuleHandleW(L"ntdll"));

//inline auto const rtl_get_full_path_name_ustr(reinterpret_cast<std::uint32_t (*)(PUNICODE_STRING,ULONG,PWSTR,PCWSTR*,PBOOLEAN,rtl_path_and_unknown*)>(GetProcAddress(ntdll_module_handle, "RtlGetFullPathName_Ustr")));

inline auto const rtl_get_full_path_name_ustr_ex(reinterpret_cast<std::uint32_t (*)(PUNICODE_STRING,PUNICODE_STRING,PUNICODE_STRING,PUNICODE_STRING*
,PSIZE_T,PBOOLEAN,rtl_path_type*,PSIZE_T)>(GetProcAddress(ntdll_module_handle, "RtlGetFullPathName_UstrEx")));

//Avoid Linking
inline auto const nt_create_file(reinterpret_cast<decltype(NtCreateFile)*>(GetProcAddress(ntdll_module_handle, "NtCreateFile")));

inline auto const nt_close(reinterpret_cast<decltype(NtClose)*>(GetProcAddress(ntdll_module_handle, "NtClose")));
}

}

class nt_file
{
	HANDLE handle;
	void closeimpl() noexcept
	{
		if(handle)
			nt_details::nt_close(handle);
	}
public:
	using char_type = char;
	using native_handle_type = HANDLE;
	template<typename ...Args>
	nt_file(native_interface_t,Args&& ...args)
	{
		auto status(nt_details::nt_create_file(std::addressof(handle),std::forward<Args>(args)...));
		if(status)
			throw nt_error(status);
	}
//	template<std::size_t om>
	nt_file(std::string_view filename)//,open_interface_t<om>)
	{
		auto unicode_filename(fast_io::utf8_to_unicode<std::wstring>(filename));
//https://stackoverflow.com/questions/7430959/how-to-make-createfile-as-fast-as-possible
		if(std::numeric_limits<USHORT>::max()<(unicode_filename.size()*sizeof(wchar_t)))
			throw std::runtime_error("unicode file name too long");
		OBJECT_ATTRIBUTES Oa{};
		IO_STATUS_BLOCK IoSb{};
		auto ushort_size(static_cast<USHORT>(unicode_filename.size()*sizeof(wchar_t)));
		UNICODE_STRING usfnm{ushort_size,ushort_size,unicode_filename.data()};
		std::size_t bytes_required(0);
		{
		nt_details::rtl_path_type path_type{};
		auto status(nt_details::rtl_get_full_path_name_ustr_ex(std::addressof(usfnm),nullptr,nullptr,nullptr,nullptr,nullptr,std::addressof(path_type),std::addressof(bytes_required)));
		if(status!=3221225507)
			throw nt_error(status);
		}
		if(std::numeric_limits<USHORT>::max()<bytes_required)
			throw std::runtime_error("full file name too long");
		std::wstring output_wstring(bytes_required>>1,0);
		UNICODE_STRING output_buffer{static_cast<USHORT>(output_wstring.size()),static_cast<USHORT>(output_wstring.size()),output_wstring.data()};
		nt_details::rtl_path_type path_type{};
		PUNICODE_STRING fullnameout(nullptr);
		auto status(nt_details::rtl_get_full_path_name_ustr_ex(std::addressof(usfnm),std::addressof(output_buffer),nullptr,std::addressof(fullnameout),nullptr,nullptr,std::addressof(path_type),std::addressof(bytes_required)));
//		PCWSTR ShortName{};
//		auto status(nt_details::rtl_get_full_path_name_ustr(std::addressof(usfnm),bytes_required,unicode_filename.data(),std::addressof(ShortName),nullptr,std::addressof(path_type)));
		if(status)
			throw nt_error(status);
		fast_io::unicode_view<decltype(fast_io::out),wchar_t> view(fast_io::out);
		fast_io::println(view,output_wstring);
		Oa.Length = sizeof(Oa);
		Oa.Attributes = OBJ_CASE_INSENSITIVE;
/*		auto status(nt_details::nt_create_file(std::addressof(handle),GENERIC_WRITE,std::addressof(Oa),std::addressof(IoSb),0,FILE_ATTRIBUTE_NORMAL,0,FILE_CREATE, FILE_NON_DIRECTORY_FILE,nullptr,0));
		if(status)
			throw nt_error(status);*/
	}
	nt_file(nt_file const&) = delete;
	nt_file& operator=(nt_file const&) = delete;
	nt_file(nt_file&& b) noexcept:handle(b.handle)
	{
		b.handle = nullptr;
	}
	nt_file& operator=(nt_file&& b) noexcept
	{
		if(std::addressof(b)!=this)
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