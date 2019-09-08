#pragma once

#include<wtypes.h>
#ifdef interface
#undef interface			//what a joke. Who did this?
#endif

namespace fast_io
{
class win32_error : public std::runtime_error
{
	static std::string format_get_last_error(DWORD error)
	{
		if (error)
		{
			char *lpMsgBuf;
			auto bufLen(FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, nullptr));
			if (bufLen)
			{
				std::unique_ptr<char,decltype(LocalFree)*> up(lpMsgBuf,LocalFree);
				return std::string(up.get(), up.get()+bufLen);
			}
		}
		return std::string();
	}
public:
	explicit win32_error(DWORD const& error = GetLastError()):std::runtime_error(format_get_last_error(error)){}
};
}