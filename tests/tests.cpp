#include <gtest/gtest.h>
#include <time.h>

extern "C" {
    #include "lib_includes.h"
    #include "dlib_includes.h"
}

int main(int argc, char **argv) {
    ::testing::GTEST_FLAG(repeat) = 5;

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}


// STATIC LIBRARY

long mtime() {
    struct timespec t{};

    clock_gettime(CLOCK_REALTIME, &t);
    long mt = (long)t.tv_sec * 1000 + t.tv_nsec / 1000000;
    return mt;
}


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
    printf("Static library takes %ld ms", t);
}

