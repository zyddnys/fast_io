This is a new Experimental library to replace iostream and cstdio based on C++ 2a feature concepts. Currently, the only compiler which supports Concepts TS is GCC.

Since C++ 20 has not been released. No standard supporting libraries for concepts, which means a lot of Concepts Definitions are ugly. It will be changed after C++ 20 is officially published.

Design goal.

1. Exception Safe & Exception neutral
2. As fast as possible. As close to system call as possible.
3. Zero Overhead.
4. As safe as possible.
5. Binary serialization for trivially copyable types and C++ standard library containers. Serialization methods like JSON are slow and tedious for machines. read/write
6. Native mutex support
7. Native Unicode/UTF-8 support
8. Compatible with C stdio/ C++ iostream
9. Native Handle Interface
10. support C style io format (fprint). C++ style io format (<<,>>). Basic Language format (print, scan)
11. Compilation time open mode parse. Supports C style open mode and C++ style open mode.
12. No internal iomanip states (since it creates security issues)
13. Extremely easy to support custom devices
14. No stupid stuff like std::endl
15. No integrated locale

Future Plan After C++ 20
1. Module support
2. Coroutine support for async IO
3. No traits_type and EOF shit if this proposal would be released
Zero-overhead deterministic exceptions: Throwing values
http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0709r0.pdf
4. Network handle support with coroutine. (NOT ASIO library)
NETWORK SUPPORT IS MORE IMPORTANT IN MODERN WORLD

Possible Stuff in the future
1. Cryptography
2. Compression/Decompression

I want this to be in the C++ standard library in the future. :)

How to use this library? Please see examples in the examples folder.

compile option:
g++ -o example example.cc -O2 -std=c++17 -fconcepts

Compiler recommendation under windows:

https://gcc-mcf.lhmouse.com/



Benchmark
root@XXXXXX:/mnt/d/hg/fast_io/tests# ./i
std::FILE*:     1.03459230s
std::ofstream:  0.59182820s
std::ofstream with tricks:      0.39233580s
obuf:   0.13328110s
obuf_mutex:     0.13685030s