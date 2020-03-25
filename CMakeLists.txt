cmake_minimum_required(VERSION 3.9)
project(TP_C_task_2 C)

set(CMAKE_C_FLAGS "-Wall -Werror -Wpedantic")
set(CMAKE_C_STANDARD 99)

#----------------------------------
set (DEFAULT_MODULE 1024)
set (DEFAULT_ARRAY_SIZE 30000000)
set (MEASURE_TIME_ITERATIONS 10)
set (DEFAULT_INPUT_TXT "data_test_input.txt")
set (DEFAULT_ARRAY_VALUE_FOR_TEST 3)

configure_file(./include/config.h.in ./include/config.h)
#----------------------------------

find_package(Threads)
set(CMAKE_CXX_FLAGS -pthread)

#gcov
set(CMAKE_C_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "-lgcov --coverage")

include_directories(include src libraries)

add_executable(static_test src/static_time_measure.c)
add_executable(dynamic_test src/dynamic_time_measure.c)

add_library(timer_lib STATIC src/time_test_funcs.c)

add_library(static_lib STATIC libraries/static_library.c)
add_library(dynamic_lib MODULE libraries/dynamic_library.c)

target_link_libraries(static_test static_lib timer_lib ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(dynamic_test timer_lib ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})


enable_testing()

add_subdirectory(googletest)
add_subdirectory(unit_tests)
