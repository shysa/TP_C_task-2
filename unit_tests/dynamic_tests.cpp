#include <gtest/gtest.h>

extern "C" {
#include "lib_includes.h"
#include "time_test_funcs.h"

#include <dlfcn.h>
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// DYNAMIC LIBRARY SECTION

class FunctionList {
public:
    void *library;
    void* (*get_array_memory)(size_t size);
    void* (*init_array)(FILE *input, int *array, size_t size);
    void* (*count_elementary_sum)(const int *array, size_t size, int module);
    void* (*free_array_memory)(int *array);
    void* (*init_array_with_same_default_value)(int *array, size_t size);
};

class FunctionListTest : public ::testing::Test {
protected:
    void SetUp() override {
        functionList.library = dlopen("../libdynamic_lib.so", RTLD_LAZY);
        *(void**)(&functionList.get_array_memory) = dlsym(functionList.library, "get_array_memory");
        *(void**)(&functionList.init_array) = dlsym(functionList.library, "init_array");
        *(void**)(&functionList.init_array_with_same_default_value) = dlsym(functionList.library, "init_array_with_same_default_value");
        *(void**)(&functionList.count_elementary_sum) = dlsym(functionList.library, "count_elementary_sum");
        *(void**)(&functionList.free_array_memory) = dlsym(functionList.library, "free_array_memory");
    }
    void TearDown() override {
        dlclose(functionList.library);
    }

    FunctionList functionList{};
};

TEST_F(FunctionListTest, GetArrayMemorySuccess) {
    size_t size = 30;
    int* array = nullptr;

    array = (int *)functionList.get_array_memory(size);

    EXPECT_TRUE(array);
    functionList.free_array_memory(array);
}

TEST_F(FunctionListTest, InitNonexistingArray) {
    size_t size = 100;
    int* array = nullptr;

    EXPECT_TRUE(functionList.init_array_with_same_default_value(array, size));
}

TEST_F(FunctionListTest, InitExistingArray) {
    size_t size = 30;
    int* array = nullptr;

    array = (int *)functionList.get_array_memory(size);
    EXPECT_TRUE(array);

    EXPECT_FALSE(functionList.init_array_with_same_default_value(array, size));
    EXPECT_EQ(array[0], ARRAY_VALUE_FOR_SIMPLE_INIT);

    functionList.free_array_memory(array);
}

TEST_F(FunctionListTest, InitNonexistingFileArray) {
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

    EXPECT_TRUE(functionList.init_array(file, array, size));

    fclose(file);
}

TEST_F(FunctionListTest, InitExistingFileArray) {
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

    array = (int *)functionList.get_array_memory(size);
    EXPECT_TRUE(array);

    EXPECT_FALSE(functionList.init_array(file, array, 15));
    for (int i = 0; i < 15; i++) {
        EXPECT_EQ(array[i], data[i]);
    }

    functionList.free_array_memory(array);

    fclose(file);
}

TEST_F(FunctionListTest, InitWrongDataFileArray) {
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

    array = (int *)functionList.get_array_memory(size);
    EXPECT_TRUE(array);

    EXPECT_TRUE(functionList.init_array(file, array, 5));

    fclose(file);
}

TEST_F(FunctionListTest, CalcElementarySumNonexistingArray) {
    size_t size = 30;
    int* array = nullptr;
    int sum = 0;
    int module = 20;

    EXPECT_TRUE((void *)functionList.count_elementary_sum(array, size, module));
}

TEST_F(FunctionListTest, CalcElementarySum) {
    size_t size = 30;
    int* array = nullptr;
    int module = 20;

    array = (int *)functionList.get_array_memory(size);
    EXPECT_TRUE(array);

    functionList.init_array_with_same_default_value(array, size);

    long sum = 0;
    sum = (long)(functionList.count_elementary_sum(array, size, module));

    EXPECT_LT(sum, module);

    functionList.free_array_memory(array);
}

TEST_F(FunctionListTest, FreeArrayMemory) {
    size_t size = 30;
    int* array = nullptr;

    array = (int *)functionList.get_array_memory(size);
    EXPECT_TRUE(array);

    EXPECT_FALSE(functionList.free_array_memory(array));
}

TEST_F(FunctionListTest, FreeNonexistingArrayMemory) {
    size_t size = 0;
    int* array = nullptr;

    EXPECT_TRUE(functionList.free_array_memory(array));
}