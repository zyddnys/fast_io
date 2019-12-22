#include"../../include/fast_io_c_interface.h"

int main()
{
	void* out_handle=NULL;
	cxx_fast_io_bufferred_acquire_file(&out_handle,"hello.txt","wb");
	cxx_fast_io_bufferred_print_c_str(out_handle,"Hello World\n");
	cxx_fast_io_bufferred_release(out_handle);
}
