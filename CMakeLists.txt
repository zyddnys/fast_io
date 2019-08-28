project(fastio)
find_package(Threads)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -fconcepts")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fconcepts")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fconcepts")
set(CMAKE_CXX_STANDARD 17)

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

cmake_minimum_required(VERSION 3.10)