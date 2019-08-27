if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()
project(fastio)

set(CMAKE_CXX_FLAGS "-fconcepts -Wall -Wextra")
set(CMAKE_CXX_STANDARD 17)

add_executable(compile "./examples/0000.Hello World/compile.cc")
add_executable(hello_world "./examples/0000.Hello World/hello_world.cc")
add_executable(hello_world_format "./examples/0000.Hello World/hello_world_format.cc")
add_executable(hello_world1 "./examples/0000.Hello World/hello_world1.cc")
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

add_executable(compile_time_open_mode "./examples/0006.compile_time_open_mode/compile_time_open_mode.cc")
add_executable(compile_time_c_style_open_mode "./examples/0006.compile_time_open_mode/compile_time_c_style_open_mode.cc")

add_executable(setw "./examples/0010.manip/setw.cc")