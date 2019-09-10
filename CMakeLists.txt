project(fastio)
find_package(Threads)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
add_definitions(-fconcepts)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
add_definitions(/experimental:concepts)
#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
else()
message(FATAL_ERROR "Your compiler does not support concepts lite.")
endif()

set(CMAKE_CXX_STANDARD 20)

add_executable(compile "./examples/0000.Hello World/compile.cc")
add_executable(hello_world "./examples/0000.Hello World/hello_world.cc")
add_executable(hello_world_format "./examples/0000.Hello World/hello_world_format.cc")
#add_executable(hello_world1 "./examples/0000.Hello World/hello_world1.cc")
add_executable(hello_world2 "./examples/0000.Hello World/hello_world2.cc")
add_executable(hello_world3 "./examples/0000.Hello World/hello_world3.cc")

add_executable(file_hello_world "./examples/0001.file io/file_hello_world.cc")
add_executable(mutex_file_hello_world "./examples/0001.file io/mutex_file_hello_world.cc")
add_executable(open_none_existing_file "./examples/0001.file io/open_none_existing_file.cc")

add_executable(a+b "./examples/0003.a+b/a+b.cc")
add_executable(getline "./examples/0002.input string/getline.cc")
add_executable(getwhole "./examples/0002.input string/getwhole.cc")

add_executable(output_10M_size_t "./examples/0021.benchmark/output_10M_size_t.cc")
add_executable(input_10M_size_t "./examples/0021.benchmark/input_10M_size_t.cc")
add_executable(input_10M_size_t_until_eof "./examples/0021.benchmark/input_10M_size_t_until_eof.cc")

add_executable(hexa+b "./examples/0018.base/hexa+b.cc")
add_executable(base "./examples/0018.base/base.cc")
add_executable(binary_serialization "./examples/0008.binary_serialization/binary_serialization.cc")
add_executable(custom_binary_serialization_type "./examples/0008.binary_serialization/custom_binary_serialization_type.cc")
add_executable(random_password "./examples/0014.random_password/random_password.cc")
add_executable(unsigned_view "./examples/0017.unsigned_view/unsigned_view.cc")

add_executable(string_to_size_t "./examples/0004.convert/string_to_size_t.cc")
add_executable(compile_time_open_mode "./examples/0006.compile_time_open_mode/compile_time_open_mode.cc")
add_executable(compile_time_c_style_open_mode "./examples/0006.compile_time_open_mode/compile_time_c_style_open_mode.cc")

add_executable(dynamic "./examples/0020.dynamic/dynamic.cc")
add_executable(setw "./examples/0010.manip/setw.cc")

add_executable(manually_lockguard "./examples/0022.mutex/manually_lockguard.cc")

add_executable(any_character_flush "./examples/0023.flush/any_character_flush.cc")
add_executable(change_flush_state "./examples/0023.flush/change_flush_state.cc")
add_executable(immediately_flush "./examples/0023.flush/immediately_flush.cc")
add_executable(line_flush "./examples/0023.flush/line_flush.cc")
add_executable(no_tie_immediately_line_flush "./examples/0023.flush/no_tie_immediately_line_flush.cc")

add_executable(multithreads_log "./examples/0024.multithreads/multithreads_log.cc")
target_link_libraries( multithreads_log ${CMAKE_THREAD_LIBS_INIT} )
add_executable(multithreads_log_line_flush "./examples/0024.multithreads/multithreads_log_line_flush.cc")
target_link_libraries( multithreads_log_line_flush ${CMAKE_THREAD_LIBS_INIT} )

add_executable(multithread_sync "./examples/0025.multiprocesses/multithread_sync.cc")
target_link_libraries(multithread_sync ${CMAKE_THREAD_LIBS_INIT} )
add_executable(ofsync "./examples/0025.multiprocesses/ofsync.cc")
add_executable(osync "./examples/0025.multiprocesses/osync.cc")
add_executable(run_mt_sync "./examples/0025.multiprocesses/run_mt_sync.cc")
target_link_libraries(run_mt_sync ${CMAKE_THREAD_LIBS_INIT} )
add_executable(addressof "./examples/0027.pointer/addressof.cc")

add_executable(emoji "./examples/0012.ucs/emoji.cc")
add_executable(ucs_view "./examples/0012.ucs/ucs_view.cc")
add_executable(test_ucs "./examples/0012.ucs/test_ucs.cc")
add_executable(hello_ucs_world "./examples/0012.ucs/hello_ucs_world.cc")

add_executable(define_custom_type "./examples/0007.define_custom_type/define_custom_type.cc")
add_executable(define_custom_type2 "./examples/0007.define_custom_type/define_custom_type2.cc")
add_executable(pipe "./examples/0028.pipe/pipe.cc")

add_executable(text_view "./examples/0030.text_view/text_view.cc")
add_executable(read_text_view "./examples/0030.text_view/read_text_view.cc")

add_executable(ifstream "./examples/0031.iostream/ifstream.cc")
add_executable(ofstream "./examples/0031.iostream/ofstream.cc")
add_executable(sstream "./examples/0031.iostream/sstream.cc")
add_executable(stream_view "./examples/0031.iostream/stream_view.cc")
add_executable(streambuf_view "./examples/0031.iostream/streambuf_view.cc")

add_executable(scan_int_until_eof "./examples/0032.eof/scan_int_until_eof.cc")

add_executable(client "./examples/0033.socket/client.cc")
add_executable(server "./examples/0033.socket/server.cc")

add_executable(double_default "./examples/0009.floating point/double_default.cc")
add_executable(double_scientific "./examples/0009.floating point/double_scientific.cc")
add_executable(double_fixed "./examples/0009.floating point/double_fixed.cc")
add_executable(double_fixed_setw "./examples/0009.floating point/double_fixed_setw.cc")
add_executable(integer_scientific "./examples/0009.floating point/integer_scientific.cc")
add_executable(floating_view "./examples/0009.floating point/floating_view.cc")
cmake_minimum_required(VERSION 3.10)