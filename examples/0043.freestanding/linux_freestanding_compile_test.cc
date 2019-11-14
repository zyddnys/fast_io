#include"../../include/fast_io_core.h"

//g++ -o linux_freestanding_compile_test linux_freestanding_compile_test.cc -O3 -std=c++2a -fconcepts -nostartfiles -ffreestanding
//Compilation success

extern "C"
{
void _start() noexcept
{
_Exit(0);
}
}