extern "C"
{
/*void cxx_fast_io_bufferred_release(void* deviceptr) CXX_FAST_IO_NOEXCEPT;
int cxx_fast_io_bufferred_acquire_file(void** generated_device,char const* filename,char const* open_mode) CXX_FAST_IO_NOEXCEPT;
int cxx_fast_io_bufferred_print_c_str(void* device,char const* c_str) CXX_FAST_IO_NOEXCEPT;
int cxx_fast_io_bufferred_put(void* device,char ch) CXX_FAST_IO_NOEXCEPT;*/
fn cxx_fast_io_bufferred_release(handle:*mut std::os::raw::c_void)->std::os::raw::c_int;
fn cxx_fast_io_bufferred_acquire_file(handle:*mut * mut std::os::raw::c_void,b:*const std::os::raw::c_char,e:*const std::os::raw::c_char)->std::os::raw::c_int;
fn cxx_fast_io_bufferred_print_double(handle:*mut std::os::raw::c_void,b:f64)->std::os::raw::c_int;
fn cxx_fast_io_bufferred_print_size_t(handle:*mut std::os::raw::c_void,b:usize)->std::os::raw::c_int;
fn cxx_fast_io_bufferred_print_ptrdiff_t(handle:*mut std::os::raw::c_void,b:isize)->std::os::raw::c_int;
fn cxx_fast_io_bufferred_put(handle:*mut std::os::raw::c_void,b:std::os::raw::c_char)->std::os::raw::c_int;
}

struct CxxFastIoBufferredHandle
{
	handle:*mut std::os::raw::c_void
}


impl CxxFastIoBufferredHandle
{
fn new(filename:std::ffi::CString,filemode:std::ffi::CString) -> CxxFastIoBufferredHandle
{
	let mut hd:*mut std::os::raw::c_void=std::ptr::null_mut();
unsafe{
	cxx_fast_io_bufferred_acquire_file(&mut hd as *mut *mut std::os::raw::c_void,filename.as_ptr(),filemode.as_ptr());
	return CxxFastIoBufferredHandle{handle:hd};
};

}
}
#[inline]
fn print_f64(h:&mut CxxFastIoBufferredHandle,d:f64)
{
unsafe
{
	cxx_fast_io_bufferred_print_double(h.handle,d);
}
}
#[inline]
fn print_usize(h:&mut CxxFastIoBufferredHandle,d:usize)
{
unsafe
{
	cxx_fast_io_bufferred_print_size_t(h.handle,d);
}
}
#[inline]
fn print_isize(h:&mut CxxFastIoBufferredHandle,d:isize)
{
unsafe
{
	cxx_fast_io_bufferred_print_ptrdiff_t(h.handle,d);
}
}
#[inline]
fn put(h:&mut CxxFastIoBufferredHandle,d:std::os::raw::c_char)
{
unsafe
{
	cxx_fast_io_bufferred_put(h.handle,d);
}
}
impl Drop for CxxFastIoBufferredHandle
{
fn drop(&mut self)
{
unsafe
{
	cxx_fast_io_bufferred_release(self.handle);
}
}
}

fn main()
{
	let mut p=CxxFastIoBufferredHandle::new(
		std::ffi::CString::new(b"rust_call_fast_io_c_interface.txt".to_vec()).expect("CString::new failed"),
		std::ffi::CString::new(b"wb".to_vec()).expect("CString::new failed"));
	let mut x:f64=0.2341242365;
	while x<10000000.0
	{
		print_f64(&mut p,x);
		put(&mut p,10);
		x=x+1.0;
	}
}