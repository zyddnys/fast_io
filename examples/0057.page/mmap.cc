#include"../../include/fast_io_device.h"
#include"../../include/fast_io.h"
#include <sys/mman.h>

template<typename... Args>
inline void* mymmap(Args&& ...args)
{
	auto p(mmap(std::forward<Args>(args)...));
	if(p==MAP_FAILED)
		throw std::system_error(errno,std::generic_category());
	return p;
}
int main()
try
{
	fast_io::osystem_file osf("abc.txt");
	auto ptr(mymmap(nullptr,2,PROT_READ|PROT_WRITE,MAP_PRIVATE,osf.native_handle(),0));

}
catch(std::exception const& e)
{
	println(fast_io::err,e);
}