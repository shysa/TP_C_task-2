project(tests)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)


find_package(Threads)

include_directories(../googletest/googletest/include)
include_directories(../library_static ../libraries ../include)

add_executable(Static_Tests_run static_tests.cpp)
add_executable(Dynamic_Tests_run dynamic_tests.cpp)

target_link_libraries(Static_Tests_run gtest gtest_main timer_lib static_lib)
target_link_libraries(Dynamic_Tests_run gtest gtest_main ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

add_test(NAME Static_Tests_run COMMAND Static_Tests_run)
add_test(NAME Dynamic_Tests_run COMMAND Dynamic_Tests_run)
