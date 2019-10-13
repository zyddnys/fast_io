cmake_minimum_required(VERSION 3.15)
project(fastio_examples)
find_package(Threads)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
add_definitions(/experimental:concepts)
#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10)
message(FATAL_ERROR "fast_io no longer supports GCC < 10 version because of incomplete C++ standard library support for concepts lite. Please build the master version of GCC from its official or mirror repo. Or use Visual Studio Preview")
endif()

add_definitions(-fconcepts -ffast-math -Wall -Wextra)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
add_definitions(-Xclang -fconcepts-ts -ffast-math -stdlib=libc++ -lc++abi)
endif()

set(CMAKE_CXX_STANDARD 20)

add_executable(compile "./0000.Hello World/compile.cc")

add_executable(hello_world "./0000.Hello World/hello_world.cc")

add_executable(hello_world_format "./0000.Hello World/hello_world_format.cc")
#add_executable(hello_world1 "./0000.Hello World/hello_world1.cc")
add_executable(hello_world2 "./0000.Hello World/hello_world2.cc")
add_executable(hello_world3 "./0000.Hello World/hello_world3.cc")
add_executable(file_hello_world "./0001.file io/file_hello_world.cc")
add_executable(mutex_file_hello_world "./0001.file io/mutex_file_hello_world.cc")
add_executable(open_none_existing_file "./0001.file io/open_none_existing_file.cc")

add_executable(a+b "./0003.a+b/a+b.cc")
add_executable(getline "./0002.input string/getline.cc")
add_executable(getwhole "./0002.input string/getwhole.cc")


add_executable(hexa+b "./0018.base/hexa+b.cc")
add_executable(base "./0018.base/base.cc")

add_executable(binary_serialization "./0008.binary_serialization/binary_serialization.cc")
add_executable(custom_binary_serialization_type "./0008.binary_serialization/custom_binary_serialization_type.cc")

add_executable(random_password "./0014.random_password/random_password.cc")
add_executable(unsigned_view "./0017.unsigned_view/unsigned_view.cc")

add_executable(string_to_size_t "./0004.convert/string_to_size_t.cc")
add_executable(compile_time_open_mode "./0006.compile_time_open_mode/compile_time_open_mode.cc")
add_executable(compile_time_c_style_open_mode "./0006.compile_time_open_mode/compile_time_c_style_open_mode.cc")

add_executable(dynamic "./0020.dynamic/dynamic.cc")
add_executable(setw "./0010.manip/setw.cc")

add_executable(manually_lockguard "./0022.mutex/manually_lockguard.cc")

add_executable(any_character_flush "./0023.flush/any_character_flush.cc")
add_executable(change_flush_state "./0023.flush/change_flush_state.cc")
add_executable(immediately_flush "./0023.flush/immediately_flush.cc")
add_executable(line_flush "./0023.flush/line_flush.cc")
add_executable(no_tie_immediately_line_flush "./0023.flush/no_tie_immediately_line_flush.cc")

add_executable(multithreads_log "./0024.multithreads/multithreads_log.cc")
target_link_libraries( multithreads_log ${CMAKE_THREAD_LIBS_INIT} )
add_executable(multithreads_log_line_flush "./0024.multithreads/multithreads_log_line_flush.cc")
target_link_libraries( multithreads_log_line_flush ${CMAKE_THREAD_LIBS_INIT} )

add_executable(multithread_sync "./0025.multiprocesses/multithread_sync.cc")
target_link_libraries(multithread_sync ${CMAKE_THREAD_LIBS_INIT} )
add_executable(ofsync "./0025.multiprocesses/ofsync.cc")
add_executable(osync "./0025.multiprocesses/osync.cc")
add_executable(run_mt_sync "./0025.multiprocesses/run_mt_sync.cc")
target_link_libraries(run_mt_sync ${CMAKE_THREAD_LIBS_INIT} )
add_executable(addressof "./0027.pointer/addressof.cc")
add_executable(pipe "./0028.pipe/pipe.cc")

add_executable(emoji "./0012.ucs/emoji.cc")
add_executable(ucs "./0012.ucs/ucs.cc")
add_executable(test_ucs "./0012.ucs/test_ucs.cc")
#add_executable(hello_ucs_world "./0012.ucs/hello_ucs_world.cc")
add_executable(input_10M_size_t "./0021.benchmark/input_10M_size_t.cc")
add_executable(input_10M_size_t_until_eof "./0021.benchmark/input_10M_size_t_until_eof.cc")
add_executable(output_10M_size_t "./0021.benchmark/output_10M_size_t.cc")
add_executable(output_10M_double "./0021.benchmark/output_10M_double.cc")
add_executable(input_10M_double "./0021.benchmark/input_10M_double.cc")
add_executable(input_10M_size_t_as_string "./0021.benchmark/input_10M_size_t_as_string.cc")
add_executable(charconv "./0021.benchmark/charconv.cc")

add_executable(client "./0033.socket/client.cc")
add_executable(server "./0033.socket/server.cc")

add_executable(text_view "./0030.text_view/text_view.cc")
add_executable(read_text_view "./0030.text_view/read_text_view.cc")

add_executable(ifstream "./0031.iostream/ifstream.cc")
add_executable(ofstream "./0031.iostream/ofstream.cc")
add_executable(sstream "./0031.iostream/sstream.cc")
add_executable(streambuf_view "./0031.iostream/streambuf_view.cc")

add_executable(scan_int_until_eof "./0032.eof/scan_int_until_eof.cc")



add_executable(double_default "./0009.floating point/double_default.cc")
add_executable(double_scientific "./0009.floating point/double_scientific.cc")
add_executable(double_fixed "./0009.floating point/double_fixed.cc")
add_executable(double_fixed_setw "./0009.floating point/double_fixed_setw.cc")
add_executable(integer_scientific "./0009.floating point/integer_scientific.cc")
add_executable(floating_view "./0009.floating point/floating_view.cc")
add_executable(one_time_pad "./0034.crypto/one_time_pad.cc")
add_executable(my_copy "./0035.transmit/my_copy.cc")

add_executable(movable "./0036.concepts_check/movable.cc")