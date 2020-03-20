cmake_minimum_required(VERSION 3.9)
project(TP_C_task_2 C)

set(CMAKE_C_FLAGS "-Wall -Werror -Wpedantic")
set(CMAKE_C_STANDARD 99)

find_package(Threads)
set(CMAKE_CXX_FLAGS -pthread)

#gcov
set(CMAKE_C_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "-lgcov --coverage")

include_directories(include src library_static library_dynamic)

add_executable(main src/main.c)
add_library(timer_lib STATIC src/timer.c)

add_library(static_lib STATIC library_static/library.c)
#add_library(dynamic_lib MODULE library_dynamic/library_dynamic.c)
#add_library(static_lib STATIC library_dynamic/library_dynamic.c)

target_link_libraries(main static_lib timer_lib ${CMAKE_THREAD_LIBS_INIT})
#target_link_libraries(main ${CMAKE_DL_LIBS})

enable_testing()

add_subdirectory(googletest)
add_subdirectory(tests)
