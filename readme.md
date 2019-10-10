This is a new header-only (module only in the future) Experimental library to replace iostream and cstdio based on C++ 2a feature concepts. Currently, the only compiler which supports Concepts TS is GCC. Supports VC preview as well. It can compile on clang concepts experimental. I have tested all the compilers.

It can even work on android !! 

## Documents
./doxygen/html/index.html

Since C++ 20 has not been released. No standard supporting libraries for concepts, which means a lot of Concepts Definitions are ugly. It will be changed after C++ 20 is officially published.

## Design goal.
  0. Custom Devices
  1. Exception Safe & Exception neutral
  2. As fast as possible. As close to system call as possible.
  3. Zero Overhead.
  4. As safe as possible.
  5. Binary serialization for trivially copyable types and C++ standard library containers. Serialization methods like JSON are slow and tedious for machines. read/write
  6. std::mutex mutex stream
  7. Unicode/UTF-8 support
  8. Compatible with C stdio/ C++ iostream
  9. Native Handle Interface
  10. support C style io format (fprint). Basic/Lua/Python/etc format (print, scan). NO LONGER SUPPORTS C++ style io format (<<,>>) since they are bad.
  11. Compilation time open mode parse. Supports C style open mode and C++ style open mode.
  12. No internal iomanip states (since it creates security issues)
  13. Extremely easy to support custom devices
  14. No stupid stuff like std::endl
  15. No integrated locale
  16. No traits_type and EOF shit
  17. All fast_io devices can be natively put in C++ containers. std::vector<fast_io::obuf> is valid
  18. Providing RAII for FILE*&POSIX file id
  19. Dynamic Type Support
  20. Pipe line support
  21. Zero copy IO
  22. Cryptography (Under construction) to replace openssl

## Future Plan After C++ 20
  1. Module support
  2. Coroutine support for async IO
  3. if this proposal would be released. Probably remove more stuffs here
     Zero-overhead deterministic exceptions: Throwing values
     http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0709r0.pdf
  4. Network handle support with coroutine. (NOT ASIO library)
     NETWORK SUPPORT IS BECOMING MORE AND MORE IMPORTANT IN MODERN WORLD
  5. Partial Freestanding mode

## Possible Stuff in the future
  1. Compression/Decompression
  2. Interfaces for cloud computing algorithms like MapReduce

I want this to be in the C++ standard library in the future. :)

## How to use this library? 
Please see examples in the examples folder.

compile option:
    
	g++ -o example example.cc -O2 -std=c++2a -fconcepts

Compiler recommendation under windows:

https://gcc-mcf.lhmouse.com/

## Benchmark

output 10000000 size_t to file
```
root@XXXXXX:/mnt/d/hg/fast_io/tests# ./i

std::FILE*:     1.03459230s

std::ofstream:  0.59182820s

std::ofstream with tricks:      0.39233580s

obuf:   0.13328110s

obuf_mutex:     0.13685030s
```

input 10000000 size_t from file
```
root@XXXXXX:/mnt/d/hg/fast_io/tests# ./j

std::FILE*:     1.04546250s

std::ifstream:  0.47838780s

ibuf:   0.08077780s
```

### Windows:
Output:
```
g++ -o output_10M_size_t output_10M_size_t.cc -O2 -std=c++2a -fconcepts

Process started (PID=1580) >>>

<<< Process finished (PID=1580). (Exit code 0)

output_10M_size_t

Process started (PID=1208) >>>

std::FILE*:	2.26901100s

std::ofstream:	1.03628600s

std::ofstream with tricks:	0.84219500s

obuf:	0.13401100s

dynamic obuf:	0.13586300s

iobuf_dynamic system_file:	0.13000100s

obuf_mutex:	0.15303500s
```

Input:
```
g++ -o input_10M_size_t input_10M_size_t.cc -O2 -std=c++2a -fconcepts

Process started (PID=11856) >>>

<<< Process finished (PID=11856). (Exit code 0)

input_10M_size_t

Process started (PID=21088) >>>

std::FILE*:	5.53888200s

std::ifstream:	1.27124600s

ibuf:	0.07400200s

dynamic standard input stream ibuf:	0.08899900s

ibuf_dynamic isystem_file:	0.07600900s
```




visual studio preview 2019
https://visualstudio.microsoft.com/vs/preview/
