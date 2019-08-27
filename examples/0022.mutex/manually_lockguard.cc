#include"../../include/fast_io.h"
#include<mutex>

int main()
{
	fast_io::obuf_mutex obm("manually_lockguard.txt");
	std::lock_guard lg(obm.mutex());			//obm.mutex() return std::mutex&
	auto& ob(obm.native_handle());
	print(ob,"HAHA. I am FREE!!!\n");
	print(ob,"Goodby, Cruel World\n");
}