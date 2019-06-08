#ifdef _POSIX_C_SOURCE
#ifndef FAST_IO_HANDLERS_POSIX_H
#define FAST_IO_HANDLERS_POSIX_H
#include<unistd.h>
#include<fcntl.h>
#include<stdexcept>
#include<string_view>
#include<cstddef>
#include<type_traits>
#include"../mode.h"
#include"../concept.h"
#include<system_error>

namespace fast_io
{
	using namespace std::string_literals;

	namespace posix_file_details
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
			static constexpr int flags()
			{
				return O_RDONLY|O_EXCL;
			}
			static constexpr int modes()
			{
				return 0;
			}

		};
		template<>
		class mode<fast_io::open_mode::out>
		{
		public:
			static constexpr int flags()
			{
				return O_WRONLY|O_CREAT;
			}
			static constexpr int modes()
			{
				return S_IRWXU | S_IRWXG | S_IRWXO;
			}
		};
		template<>
		class mode<fast_io::open_mode::app>
		{
		public:
			static constexpr int flags()
			{
				return O_APPEND|O_CREAT;
			}
			static constexpr int modes()
			{
				return 0;
			}
		};
	}

	template<typename MODE>
	class posix_file
	{
		int fd;
		void _close() noexcept
		{
			if(fd!=-1)
				close(fd);
		}
		char* _read(char *begin,char *end)
		{
			ssize_t read_bytes(::read(fd,begin,end-begin));
			if(read_bytes==-1)
				throw std::system_error(errno,std::system_category());
			return read_bytes+begin;
		}
		void _write(const char *begin,const char *end)
		{
			if(::write(fd,begin,end-begin)==-1)
				throw std::system_error(errno,std::system_category());
		}
	public:
		using char_type = char;

		using native_handle_type = int;
		posix_file(std::string_view file):
			fd(open(file.data(),posix_file_details::mode<MODE>::flags(),
			posix_file_details::mode<MODE>::modes()))
		{
			if(fd==-1)
				throw std::system_error(errno,std::generic_category());
		}
		template<typename ...Args>
		posix_file(fast_io::native_interface_t,Args&& ...args):
			fd(open(std::forward<Args>(args)...))
		{
			if(fd==-1)
				throw std::system_error(errno,std::generic_category());
		}
		posix_file(const posix_file&)=delete;
		posix_file& operator=(const posix_file&)=delete;
		posix_file(posix_file&& b) noexcept : fd(b.fd)
		{
			b.fd = -1;
		}
		posix_file& operator=(posix_file&& b) noexcept
		{
			if(&b!=this)
			{
				_close();
				fd=b.fd;
				b.fd = -1;
			}
			return *this;
		}
		native_handle_type native_handle() const
		{
			return fd;
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
			if(fsync(fd)==-1)
				throw std::system_error(errno,std::system_category());
		}
		~posix_file()
		{
			_close();
		}
	};
}
#endif
#endif