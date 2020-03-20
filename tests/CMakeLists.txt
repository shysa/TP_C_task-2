project(tests)

include_directories(../googletest/googletest/include)
include_directories(../library_static)

add_executable(Google_Tests_run tests.cpp)

target_link_libraries(Google_Tests_run gtest gtest_main static_lib)

add_test(NAME Google_Tests_run COMMAND Google_Tests_run)