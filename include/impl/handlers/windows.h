#ifdef _WIN32_WINNT
#ifndef FAST_IO_WINDOWS_H
#define FAST_IO_WINDOWS_H
#include<windows.h>
#include<string>
#include<cstddef>
#include<type_traits>
#include"../mode.h"
#include"../concept.h"
#include<system_error>
#include<memory>

namespace fast_io
{
	class win32_error : public std::system_error
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
		explicit win32_error(const DWORD& error = GetLastError()):std::system_error(std::error_code(error, std::system_category()),format_get_last_error(error)){}
	};
	
	namespace windows_file_details
	{
		template<typename T>
		class mode
		{
		public:
		};
		template<>
		class mode<fast_io::open_mode::in>
		{
		public:
			static constexpr DWORD desired_access()
			{
				return GENERIC_READ;
			}
			static constexpr DWORD share_mode()
			{
				return 0;
			}
			static constexpr LPSECURITY_ATTRIBUTES security_attributes()
			{
				return nullptr;
			}
			static constexpr DWORD creation_disposition()
			{
				return OPEN_EXISTING;
			}
			static constexpr DWORD flags_and_attributes()
			{
				return 0;
			}
			static constexpr HANDLE template_file()
			{
				return nullptr;
			}
		};
		template<>
		class mode<fast_io::open_mode::out>
		{
		public:
			static constexpr DWORD desired_access()
			{
				return GENERIC_WRITE;
			}
			static constexpr DWORD share_mode()
			{
				return 0;
			}
			static constexpr LPSECURITY_ATTRIBUTES security_attributes()
			{
				return nullptr;
			}
			static constexpr DWORD creation_disposition()
			{
				return CREATE_ALWAYS;
			}
			static constexpr DWORD flags_and_attributes()
			{
				return 0;
			}
			static constexpr HANDLE template_file()
			{
				return nullptr;
			}
		};
		
		template<>
		class mode<fast_io::open_mode::app>
		{
		public:
			static constexpr DWORD desired_access()
			{
				return GENERIC_WRITE;
			}
			static constexpr DWORD share_mode()
			{
				return 0;
			}
			static constexpr LPSECURITY_ATTRIBUTES security_attributes()
			{
				return nullptr;
			}
			static constexpr DWORD creation_disposition()
			{
				return OPEN_ALWAYS;
			}
			static constexpr DWORD flags_and_attributes()
			{
				return 0;
			}
			static constexpr HANDLE template_file()
			{
				return nullptr;
			}

		};
	}

	
	template<typename MODE>
	class windows_file
	{
		HANDLE mhandle;
		using _mode = windows_file_details::mode<MODE>;
		char* _read(char *buf_beg,char *buf_end)
		{
			DWORD bytes;
			if(!ReadFile(mhandle,buf_beg,buf_end-buf_beg,&bytes,nullptr))
				throw win32_error();
			return buf_beg+bytes;
		}
		void _write(const char *buf_beg,const char *buf_end)
		{
			DWORD bytes,v(buf_end-buf_beg);
			if(!WriteFile(mhandle,buf_beg,v,&bytes,nullptr)||bytes!=v)
				throw win32_error();
		}
		void _close()
		{
			if(mhandle!=nullptr)
				CloseHandle(mhandle);
		}
	public:
	
		using char_type = char;
		using native_handle_type = HANDLE;
		template<typename ...Args>
		windows_file(fast_io::native_interface_t,Args&& ...args):
			mhandle(CreateFile(std::forward<Args>(args)...))
		{
			if(mhandle==INVALID_HANDLE_VALUE)
				throw win32_error();
		}
		windows_file(const char* filename):
				mhandle(CreateFile(filename, _mode::desired_access(), _mode::share_mode(), _mode::security_attributes(), _mode::creation_disposition(), _mode::flags_and_attributes(), _mode::template_file()))
		{
			if(mhandle==INVALID_HANDLE_VALUE)
				throw win32_error();
		}
		windows_file(const std::string& filename):windows_file(filename.c_str()){}
		
		~windows_file()
		{
			_close();
		}
		windows_file(const windows_file&)=delete;
		windows_file& operator=(const windows_file&)=delete;
		windows_file(windows_file&& b) noexcept:mhandle(b.mhandle)
		{
			b.mhandle=nullptr;
		}
		windows_file& operator=(windows_file&& b) noexcept
		{
			if(&b!=this)
			{
				_close();
				mhandle = b.mhandle;
				b.mhandle=nullptr;
			}
			return *this;
		}
		native_handle_type native_handle() const
		{
			return mhandle;
		}
		template<Pointer T>
		T read(T begin,T end)
		{
			return reinterpret_cast<T>(_read(reinterpret_cast<char*>(begin),reinterpret_cast<char*>(end)));
		}
		void write(Pointer begin,Pointer end)
		{
			_write(reinterpret_cast<const char*>(begin),reinterpret_cast<const char*>(end));
		}
		void flush()
		{
			if(!FlushFileBuffers(mhandle))
				throw win32_error();
		}
	};
}
#endif
#endif