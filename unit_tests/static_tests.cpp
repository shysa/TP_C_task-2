#include <gtest/gtest.h>

extern "C" {
    #include "lib_includes.h"
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


// STATIC LIBRARY SECTION

TEST(StaticLibrary, GetArrayMemorySuccess) {
    size_t size = 30;
    int* array = nullptr;

    array = get_array_memory(size);
    EXPECT_TRUE(array);
    free_array_memory(array);
}

TEST(StaticLibrary, InitNonexistingArray) {
    size_t size = 100;
    int* array = nullptr;

    EXPECT_TRUE(init_array_with_same_default_value(array, size));
}

TEST(StaticLibrary, InitExistingArray) {
    size_t size = 30;
    int* array = nullptr;

    array = get_array_memory(size);
    EXPECT_TRUE(array);

    EXPECT_FALSE(init_array_with_same_default_value(array, size));
    EXPECT_EQ(array[0], ARRAY_VALUE_FOR_SIMPLE_INIT);

    free_array_memory(array);
}

TEST(StaticLibrary, InitNonexistingFileArray) {
    size_t size = 15;
    int* array = nullptr;

    const int data[15] = {
            1, 2, 3, 4, 5,
            10, 20, 30, 40, 50,
            100, 200, 300, 400, 500
    };

    FILE * file = fopen("data_test_input.txt", "w+");
    for (int value : data) {
        fprintf(file, "%d ", value);
    }
    fclose(file);

    file = fopen("data_test_input.txt", "r");

    EXPECT_TRUE(init_array(file, array, size));

    fclose(file);
}

TEST(StaticLibrary, InitExistingFileArray) {
    size_t size = 15;
    int* array = nullptr;

    const int data[15] = {
            1, 2, 3, 4, 5,
            10, 20, 30, 40, 50,
            100, 200, 300, 400, 500
    };

    FILE * file = fopen("data_test_input.txt", "w+");
    for (int value : data) {
        fprintf(file, "%d ", value);
    }
    fclose(file);

    file = fopen("data_test_input.txt", "r");

    array = get_array_memory(size);
    EXPECT_TRUE(array);

    EXPECT_FALSE(init_array(file, array, 15));
    for (int i = 0; i < 15; i++) {
        EXPECT_EQ(array[i], data[i]);
    }

    free_array_memory(array);

    fclose(file);
}

TEST(StaticLibrary, InitWrongDataFileArray) {
    size_t size = 5;
    int* array = nullptr;

    const char data[5] = {
            'a', 'b', 'c', 'd', 'e'
    };

    FILE * file = fopen("data_test_input.txt", "w+");
    for (int value : data) {
        fprintf(file, "%c ", value);
    }
    fclose(file);

    file = fopen("data_test_input.txt", "r");

    array = get_array_memory(size);
    EXPECT_TRUE(array);

    EXPECT_TRUE(init_array(file, array, 5));

    fclose(file);
}

TEST(StaticLibrary, CalcElementarySumNonexistingArray) {
    size_t size = 0;
    int* array = nullptr;
    long sum = 0;
    int module = 20;

    EXPECT_TRUE(count_elementary_sum(array, size, module));
}

TEST(StaticLibrary, CalcElementarySum) {
    size_t size = 30;
    int* array = nullptr;
    int module = 15;

    array = get_array_memory(size);
    EXPECT_TRUE(array);
    init_array_with_same_default_value(array, size);

    long sum = 0;
    sum = count_elementary_sum(array, size, module);

    EXPECT_LT(sum, module);

    free_array_memory(array);
}

TEST(StaticLibrary, FreeArrayMemory) {
    size_t size = 30;
    int* array = nullptr;

    array = get_array_memory(size);
    EXPECT_TRUE(array);

    EXPECT_FALSE(free_array_memory(array));
}

TEST(StaticLibrary, FreeNonexistingArrayMemory) {
    size_t size = 0;
    int* array = nullptr;

    EXPECT_TRUE(free_array_memory(array));
}

