#include <gtest/gtest.h>

extern "C" {
    #include "lib_includes.h"
    #include "dlib_includes.h"
    #include "timer.h"
}

int main(int argc, char **argv) {
    //::testing::GTEST_FLAG(repeat) = 5;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


// STATIC LIBRARY

TEST(StaticCreatingMassive, CountSuccess) {
    long t = mtime();

    size_t size = 0;
    int * massive = nullptr;

    massive = get_massive_memory(&size);
    ASSERT_TRUE(massive);

    EXPECT_FALSE(init_massive(massive, size));

    EXPECT_FALSE(count_elementary_sum(massive, size));

    EXPECT_FALSE(free_massive_memory(massive, size));

    t = mtime() - t;
    printf("Static library_static takes %ld ms", t);
}

