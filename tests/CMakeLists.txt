project(tests)

include_directories(../googletest/googletest/include)
include_directories(../library_static ../libraries ../include)

add_executable(Google_Tests_run tests.cpp)

target_link_libraries(Google_Tests_run gtest gtest_main timer_lib static_lib ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

add_test(NAME Google_Tests_run COMMAND Google_Tests_run)