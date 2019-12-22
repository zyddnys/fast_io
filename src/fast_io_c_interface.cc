#include"../include/fast_io_device.h"
#include<cerrno>

namespace
{

inline int lippincott_function()
{
	try
	{
		throw;
	}
	catch(std::system_error const& error)
	{
		if(error.code().category()==std::generic_category())
			return error.code().value();
		return EIO;
	}
	catch(...)
	{
		return EIO;
	}
}
}

extern "C"
{

void cxx_fast_io_release(void* d) noexcept
{
	fast_io::dynamic_stream dyn(d);
}

int cxx_fast_io_send(void* d,char const* begin,char const* end) noexcept
{
	try
	{
		fast_io::dynamic_base dyn(d);
		send(dyn,begin,end);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

int cxx_fast_io_receive(char** readed,void* d,char* begin,char* end) noexcept
{
	try
	{
		fast_io::dynamic_base dyn(d);
		*readed=receive(dyn,begin,end);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

int cxx_fast_io_flush(void* d) noexcept
{
	try
	{
		fast_io::dynamic_base dyn(d);
		flush(dyn);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

int cxx_fast_io_print_size_t(void* d,std::size_t s) noexcept
{
	try
	{
		fast_io::dynamic_base dyn(d);
		print(dyn,s);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

int cxx_fast_io_print_ptrdiff_t(void* d,std::ptrdiff_t s) noexcept
{
	try
	{
		fast_io::dynamic_base dyn(d);
		print(dyn,s);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

int cxx_fast_io_println_size_t(void* d,std::size_t s) noexcept
{
	try
	{
		fast_io::dynamic_base dyn(d);
		println(dyn,s);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

int cxx_fast_io_println_ptrdiff_t(void* d,std::ptrdiff_t s) noexcept
{
	try
	{
		fast_io::dynamic_base dyn(d);
		println(dyn,s);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

int cxx_fast_io_print_double(void* d,double s) noexcept
{
	try
	{
		fast_io::dynamic_base dyn(d);
		print(dyn,s);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}


int cxx_fast_io_print_c_str(void* d,char const* s) noexcept
{
	try
	{
		fast_io::dynamic_base dyn(d);
		print(dyn,s);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

int cxx_fast_io_acquire_stdout(void** out) noexcept
{
	try
	{
		fast_io::dynamic_base dyn(std::in_place_type<fast_io::system_io_handle>,fast_io::native_stdout_number);
		*out=dyn.opaque_base_pointer();
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}
int cxx_fast_io_acquire_stderr(void** out) noexcept
{
	try
	{
		fast_io::dynamic_base dyn(std::in_place_type<fast_io::system_io_handle>,fast_io::native_stdout_number);
		*out=dyn.opaque_base_pointer();
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}
int cxx_fast_io_acquire_stdin(void** out) noexcept
{
	try
	{
		fast_io::dynamic_base dyn(std::in_place_type<fast_io::system_io_handle>,fast_io::native_stdin_number);
		*out=dyn.opaque_base_pointer();
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

int cxx_fast_io_acquire_file(void** out,char const* name,char const* mode) noexcept
{
	try
	{
		fast_io::dynamic_base dyn(std::in_place_type<fast_io::system_file>,name,mode);
		*out=dyn.opaque_base_pointer();
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}


int cxx_fast_io_acquire_buf(void** out,char const* name,char const* mode) noexcept
{
	try
	{
		fast_io::dynamic_base dyn(std::in_place_type<fast_io::basic_iobuf<fast_io::system_file>>,name,mode);
		*out=dyn.opaque_base_pointer();
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

void cxx_fast_io_bufferred_release(void* d) noexcept
{
	delete fast_io::bit_cast<fast_io::dynamic_buf*>(d);
}

int cxx_fast_io_bufferred_acquire_file(void** out,char const* name,char const* mode) noexcept
{
	try
	{
		*out=new fast_io::dynamic_buf(std::in_place_type<fast_io::basic_iobuf<fast_io::system_file>>,name,mode);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}


int cxx_fast_io_bufferred_print_size_t(void* d,std::size_t s) noexcept
{
	try
	{
		print(*fast_io::bit_cast<fast_io::dynamic_buf*>(d),s);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

int cxx_fast_io_bufferred_print_ptrdiff_t(void* d,std::ptrdiff_t s) noexcept
{
	try
	{
		print(*fast_io::bit_cast<fast_io::dynamic_buf*>(d),s);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

int cxx_fast_io_bufferred_println_size_t(void* d,std::size_t s) noexcept
{
	try
	{
		println(*fast_io::bit_cast<fast_io::dynamic_buf*>(d),s);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

int cxx_fast_io_bufferred_println_ptrdiff_t(void* d,std::ptrdiff_t s) noexcept
{
	try
	{
		println(*fast_io::bit_cast<fast_io::dynamic_buf*>(d),s);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}


int cxx_fast_io_bufferred_print_double(void* d,double s) noexcept
{
	try
	{
		print(*fast_io::bit_cast<fast_io::dynamic_buf*>(d),s);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

int cxx_fast_io_bufferred_print_c_str(void* d,char const* s) noexcept
{
	try
	{
		print(*fast_io::bit_cast<fast_io::dynamic_buf*>(d),s);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

int cxx_fast_io_bufferred_put(void* d,char ch) noexcept
{
	try
	{
		put(*fast_io::bit_cast<fast_io::dynamic_buf*>(d),ch);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

}