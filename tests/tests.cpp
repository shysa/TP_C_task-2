#include <gtest/gtest.h>

extern "C" {
    #include "lib_includes.h"
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    testing::GTEST_FLAG(print_time) = true;
    testing::GTEST_FLAG()

    return RUN_ALL_TESTS();
}


// STATIC LIBRARY

TEST(CreatingMassive, CountSuccess) {
    size_t size = 0;
    int * massive = nullptr;

    massive = get_massive_memory(&size);
    ASSERT_TRUE(massive);

    EXPECT_FALSE(init_massive(massive, size));

    EXPECT_FALSE(count_elementary_sum(massive, size));

    EXPECT_FALSE(free_massive_memory(massive, size));

}