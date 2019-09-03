#pragma once

#include"win32_error.h"

namespace fast_io
{
namespace details
{

struct win32_open_mode
{
DWORD dwDesiredAccess=0,dwShareMode=FILE_SHARE_READ|FILE_SHARE_WRITE;
LPSECURITY_ATTRIBUTES lpSecurityAttributes=nullptr;
DWORD dwCreationDisposition=0;	//depends on EXCL
DWORD dwFlagsAndAttributes=FILE_ATTRIBUTE_NORMAL|FILE_FLAG_RANDOM_ACCESS;
};


inline constexpr win32_open_mode calculate_win32_open_mode(open::mode const &om)
{
	using namespace open;
	std::size_t value(remove_ate(om).value);
	win32_open_mode mode;
	if(value&open::app.value)
		mode.dwDesiredAccess|=FILE_APPEND_DATA;
	else if(value&open::out.value)
		mode.dwDesiredAccess|=GENERIC_WRITE;
	if(value&open::in.value)
		mode.dwDesiredAccess|=GENERIC_READ;
	if(value&open::excl.value)
	{
		mode.dwCreationDisposition=CREATE_NEW;
		if(value&open::trunc.value)
			throw std::runtime_error("cannot create new while truncating existed file");
	}
	else if (value&open::trunc.value)
		mode.dwCreationDisposition=CREATE_ALWAYS;
	else if(!(value&open::in.value))
	{
		if(value&open::app.value)
			mode.dwCreationDisposition=OPEN_ALWAYS;
		else
			mode.dwCreationDisposition=CREATE_ALWAYS;
	}
	return mode;
}
template<std::size_t om>
struct win32_file_openmode
{
	static win32_open_mode constexpr mode = calculate_win32_open_mode(om);
};
}

class win32_file
{
	HANDLE mhandle;
	void closeimpl()
	{
		if(mhandle)
			CloseHandle(mhandle);
	}
public:
	using char_type = char;
	using native_handle_type = HANDLE;
	template<typename ...Args>
	win32_file(fast_io::native_interface_t,Args&& ...args):mhandle(CreateFileW(std::forward<Args>(args)...))
	{
		if(mhandle==INVALID_HANDLE_VALUE)
			throw win32_error();
	}

	template<typename T>
	void seek(seek_type_t<T>,Integral i,seekdir s=seekdir::beg)
	{
		LONG distance_to_move_high(0);
		if(SetFilePointer(mhandle,seek_precondition<LONG,T,char_type>(i),std::addressof(distance_to_move_high),static_cast<DWORD>(s))== INVALID_SET_FILE_POINTER)
		{
			auto const last_error(GetLastError());
			if(last_error)
				throw win32_error(last_error);
		}
	}
	void seek(Integral i,seekdir s=seekdir::beg)
	{
		seek(seek_type<char_type>,i,s);
	}

	template<std::size_t om>
	win32_file(std::string_view filename,open::interface_t<om>):win32_file(fast_io::native_interface,fast_io::utf8_to_unicode<std::wstring>(filename).c_str(),
				details::win32_file_openmode<om>::mode.dwDesiredAccess,
				details::win32_file_openmode<om>::mode.dwShareMode,
				details::win32_file_openmode<om>::mode.lpSecurityAttributes,
				details::win32_file_openmode<om>::mode.dwCreationDisposition,
				details::win32_file_openmode<om>::mode.dwFlagsAndAttributes,nullptr)
	{
		if constexpr (with_ate(open::mode(om)))
			seek(0,seekdir::end);
	}
	win32_file(std::string_view filename,open::mode const& m)
	{
		auto const mode(details::calculate_win32_open_mode(m));
		if((mhandle=CreateFileW(fast_io::utf8_to_unicode<std::wstring>(filename).c_str(),
					mode.dwDesiredAccess,
					mode.dwShareMode,
					mode.lpSecurityAttributes,
					mode.dwCreationDisposition,
					mode.dwFlagsAndAttributes,nullptr))==INVALID_HANDLE_VALUE)
			throw win32_error();
		if(with_ate(m))
			seek(0,seekdir::end);
	}
	win32_file(std::string_view file,std::string_view mode):win32_file(file,fast_io::open::c_style(mode)){}
	~win32_file()
	{
		closeimpl();
	}
	win32_file(win32_file const&)=delete;
	win32_file& operator=(win32_file const&)=delete;
	win32_file(win32_file&& b) noexcept:mhandle(b.mhandle)
	{
		b.mhandle=nullptr;
	}
	win32_file& operator=(win32_file&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			closeimpl();
			mhandle = b.mhandle;
			b.mhandle=nullptr;
		}
		return *this;
	}
	native_handle_type native_handle() const
	{
		return mhandle;
	}
	template<typename Contiguous_iterator>
	Contiguous_iterator read(Contiguous_iterator begin,Contiguous_iterator end)
	{
		DWORD numberOfBytesRead;
		if(!ReadFile(mhandle,std::addressof(*begin),static_cast<DWORD>((end-begin)*sizeof(*begin)),&numberOfBytesRead,nullptr))
			throw win32_error();
		return begin+numberOfBytesRead;
	}
	template<typename Contiguous_iterator>
	void write(Contiguous_iterator cbegin,Contiguous_iterator cend)
	{
		auto nNumberOfBytesToWrite(static_cast<DWORD>((cend-cbegin)*sizeof(*cbegin)));
		DWORD numberOfBytesWritten;
		if(!WriteFile(mhandle,std::addressof(*cbegin),nNumberOfBytesToWrite,std::addressof(numberOfBytesWritten),nullptr)||
							nNumberOfBytesToWrite!=numberOfBytesWritten)
			throw win32_error();
	}
	void flush()
	{
	}
};


}