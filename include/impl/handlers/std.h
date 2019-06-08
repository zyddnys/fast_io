#ifndef FAST_IO_HANDLERS_STD_H
#define FAST_IO_HANDLERS_STD_H
#include<cstdio>
#include<stdexcept>
#include<string>
#include<cstddef>
#include<type_traits>
#include"../mode.h"
#include"../concept.h"
#include<system_error>

namespace fast_io
{
	using namespace std::string_literals;

	namespace std_file_details
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
			static const std::string& m()
			{
				static const auto r("rb"s);
				return r;
			}
		};
		template<>
		class mode<fast_io::open_mode::out>
		{
		public:
			static const std::string& m()
			{
				static const auto r("wb"s);
				return r;
			}
		};
		template<>
		class mode<fast_io::open_mode::app>
		{
		public:
			static const std::string& m()
			{
				static const auto r("ab"s);
				return r;
			}
		};
	}

	template<typename MODE>
	class std_file
	{
		
		std::FILE *fp;
		void close() noexcept
		{
			if(fp)
				std::fclose(fp);
		}
		std::size_t _read(void *buffer,std::size_t size,std::size_t count)
		{
			const std::size_t r(std::fread(buffer,size,count,fp));
			if(r==count||std::feof(fp))
				return r;
			throw std::system_error(errno,std::generic_category());
		}
		void _write(const void *buffer,std::size_t size,std::size_t count)
		{
			if(std::fwrite(buffer,size,count,fp)<count)
				throw std::system_error(errno,std::generic_category());
		}
	public:
		using char_type = char;
		using int_type = int;
		using native_handle_type = std::FILE*;
		std_file(const char* file):fp(std::fopen(file,std_file_details::mode<MODE>::m().c_str()))
		{
			if(fp==nullptr)
				throw std::system_error(errno,std::generic_category());
		}
		std_file(const std::string &s):std_file(s.c_str()){}
		template<typename ...Args>
		std_file(fast_io::native_interface_t,Args&& ...args):fp(std::fopen(std::forward<Args>(args)...))
		{
			if(fp==nullptr)
				throw std::system_error(errno,std::generic_category());
		}
		std_file(const std_file&)=delete;
		std_file& operator=(const std_file&)=delete;
		std_file(std_file&& b) noexcept : fp(b.fp)
		{
			b.fp = nullptr;
		}
		std_file& operator=(std_file&& b) noexcept
		{
			if(&b!=this)
			{
				close();
				fp=b.fp;
				b.fp = nullptr;
			}
			return *this;
		}
		native_handle_type native_handle() const
		{
			return fp;
		}
		Pointer read(Pointer begin,Pointer end)
		{
			return begin+_read(begin,sizeof(*begin),end-begin);
		}
		bool eof() const
		{
			return std::feof(fp);
		}
		operator bool() const
		{
			return !eof();
		}
		void write(Pointer begin,Pointer end)
		{
			_write(begin,sizeof(*begin),end-begin);
		}
		int_type get()
		{
			return fgetc(fp);
		}
		void put(int_type ch)
		{
			if(fputc(ch,fp)==EOF)
				throw std::system_error(errno,std::system_category());
		}
		template<typename ...Args>
		void printf(Args&& ...args)
		{
			if(std::fprintf(fp,std::forward<Args>(args)...)<0)
				throw std::system_error(errno,std::system_category());		
		}
		void flush()
		{
			if(std::fflush(fp))
				throw std::system_error(errno,std::system_category());
		}
		~std_file()
		{
			close();
		}
	};

	class std_in
	{
		std::size_t _read(void *buffer,std::size_t size,std::size_t count)
		{
			const std::size_t r(std::fread(buffer,size,count,stdin));
			if(r==count||std::feof(stdin))
				return r;
			throw std::system_error(errno,std::generic_category());
		}
	public:
		using char_type = char;
		using int_type = int;
		using native_handle_type = std::FILE*;
		native_handle_type native_handle() const
		{
			return stdin;
		}
		Pointer read(Pointer begin,Pointer end)
		{
			return begin+_read(begin,sizeof(*begin),end-begin);
		}
		int get()
		{
			return fgetc(stdin);
		}
		bool eof() const
		{
			return std::feof(stdin);
		}
	};
	class std_out
	{
		void _write(const void *buffer,std::size_t size,std::size_t count)
		{
			if(std::fwrite(buffer,size,count,stdout)<count)
				throw std::system_error(errno,std::generic_category());
		}
	public:
		using char_type = char;
		using int_type = int;
		using native_handle_type = std::FILE*;
		native_handle_type native_handle() const
		{
			return stdout;
		}
		void write(Pointer begin,Pointer end)
		{
			_write(begin,sizeof(*begin),end-begin);
		}
		void put(int ch)
		{
			if(std::fputc(ch,stdout)==EOF)
				throw std::system_error(errno,std::system_category());
		}
		template<typename ...Args>
		void printf(Args&& ...args)
		{
			if(std::printf(std::forward<Args>(args)...)<0)
				throw std::system_error(errno,std::system_category());		
		}

		void flush()
		{
			if(std::fflush(stdout))
				throw std::system_error(errno,std::system_category());
		}
	};
	class std_err
	{
		void _write(const void *buffer,std::size_t size,std::size_t count)
		{
			if(std::fwrite(buffer,size,count,stderr)<count)
				throw std::system_error(errno,std::generic_category());
		}
	public:
		using char_type = char;
		using int_type = int;
		using native_handle_type = std::FILE*;
		native_handle_type native_handle() const
		{
			return stderr;
		}
		void write(Pointer begin,Pointer end)
		{
			_write(begin,sizeof(*begin),end-begin);
		}
		void put(int ch)
		{
			if(std::fputc(ch,stderr)==EOF)
				throw std::system_error(errno,std::system_category());
		}
		template<typename ...Args>
		void printf(Args&& ...args)
		{
			if(std::fprintf(stderr,std::forward<Args>(args)...)<0)
				throw std::system_error(errno,std::system_category());		
		}

		void flush()
		{
			if(std::fflush(stderr))
				throw std::system_error(errno,std::system_category());
		}
	};
}
#endif