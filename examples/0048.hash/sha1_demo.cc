#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_hash.h"

int main(int argc,char ** argv)
{
	fast_io::sha1 sha1;
	print_flush(sha1,"hello world");
	println(fast_io::out,sha1);
//sha1 value of "hello world" is: 2aae6c35c94fcfb415dbe95f408b9ce91ee846ed
}