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


// STATIC LIBRARY SECTION

TEST(StaticLibrary, GetArrayMemorySuccess) {
    size_t size = 0;
    int* array = nullptr;

    array = get_array_memory(&size, SIZE_MB);
    EXPECT_TRUE(array);
    free_array_memory(array, size);
}

TEST(StaticLibrary, CalcArraySize) {
    size_t size = 0;
    int* array = nullptr;

    array = get_array_memory(&size, SIZE_MB);
    EXPECT_TRUE(array);

    EXPECT_TRUE(size != 0);

    free_array_memory(array, size);
}

TEST(StaticLibrary, InitNonexistingArray) {
    size_t size = 100;
    int* array = nullptr;

    EXPECT_TRUE(init_array_simple(array, size));
}

TEST(StaticLibrary, InitExistingArray) {
    size_t size = 0;
    int* array = nullptr;

    array = get_array_memory(&size, SIZE_MB);
    EXPECT_TRUE(array);

    EXPECT_FALSE(init_array_simple(array, size));
    EXPECT_EQ(array[0], ARRAY_VALUE_FOR_SIMPLE_INIT);

    free_array_memory(array, size);
}

TEST(StaticLibrary, InitNonexistingFileArray) {
    size_t size = 0;
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
    size_t size = 0;
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

    array = get_array_memory(&size, SIZE_MB);
    EXPECT_TRUE(array);

    EXPECT_FALSE(init_array(file, array, 15));
    for (int i = 0; i < 15; i++) {
        EXPECT_EQ(array[i], data[i]);
    }

    free_array_memory(array, size);

    fclose(file);
}

TEST(StaticLibrary, InitWrongDataFileArray) {
    size_t size = 0;
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

    array = get_array_memory(&size, SIZE_MB);
    EXPECT_TRUE(array);

    EXPECT_TRUE(init_array(file, array, 5));

    free_array_memory(array, size);

    fclose(file);
}

TEST(StaticLibrary, CalcElementarySumNonexistingArray) {
    size_t size = 0;
    int* array = nullptr;
    int sum = 0;

    EXPECT_TRUE(count_elementary_sum(array, size, &sum));
}

TEST(StaticLibrary, CalcElementarySum) {
    size_t size = 0;
    int* array = nullptr;

    array = get_array_memory(&size, SIZE_MB);
    EXPECT_TRUE(array);
    init_array_simple(array, size);

    int sum = 0;
    count_elementary_sum(array, size, &sum);

    EXPECT_LT(sum, MODULE);

    free_array_memory(array, size);
}

TEST(StaticLibrary, FreeArrayMemory) {
    size_t size = 0;
    int* array = nullptr;

    array = get_array_memory(&size, SIZE_MB);
    EXPECT_TRUE(array);

    EXPECT_FALSE(free_array_memory(array, size));
}

TEST(StaticLibrary, FreeNonexistingArrayMemory) {
    size_t size = 0;
    int* array = nullptr;

    EXPECT_TRUE(free_array_memory(array, size));
}


TEST(StaticLibrary, CountSuccess) {
    size_t size = 0;
    int * array = nullptr;
    int sum = 0;

    array = get_array_memory(&size, SIZE_MB);
    EXPECT_TRUE(array);

    init_array_simple(array, size);

    long t = mtime();
    count_elementary_sum(array, size, &sum);
    t = mtime() - t;

    free_array_memory(array, size);

    FILE * file = fopen("static_library_time.txt", "w+");
    fprintf(file, "%ld ms\n", t);
    fclose(file);
}


// DYNAMIC LIBRARY SECTION

class FunctionList {
public:
    void * library;
    void* (*get_array_memory)(size_t * size, const size_t size_array_mb);
    void* (*init_array)(FILE * input, int* array, size_t size);
    void* (*init_array_simple)(int* massive, const size_t size);
    void* (*count_elementary_sum)(const int * massive, const size_t size, int * sum);
    void* (*free_array_memory)(int* massive, const size_t size);
};

class FunctionListTest : public ::testing::Test {
protected:
    void SetUp() override {
        functionList.library = dlopen("./libdynamic_lib.so", RTLD_LAZY);
        *(void**)(&functionList.get_array_memory) = dlsym(functionList.library, "get_array_memory");
        *(void**)(&functionList.init_array) = dlsym(functionList.library, "init_array");
        *(void**)(&functionList.init_array_simple) = dlsym(functionList.library, "init_array_simple");
        *(void**)(&functionList.count_elementary_sum) = dlsym(functionList.library, "count_elementary_sum");
        *(void**)(&functionList.free_array_memory) = dlsym(functionList.library, "free_array_memory");
    }
    void TearDown() override {
        dlclose(functionList.library);
    }

    FunctionList functionList{};
};

TEST_F(FunctionListTest, GetArrayMemorySuccess) {
    size_t size = 0;
    int* array = nullptr;

    array = (int *)functionList.get_array_memory(&size, SIZE_MB);

    EXPECT_TRUE(array);
    functionList.free_array_memory(array, size);
}

TEST_F(FunctionListTest, CalcArraySize) {
    size_t size = 0;
    int* array = nullptr;

    array = (int *)(functionList.get_array_memory(&size, SIZE_MB));
    EXPECT_TRUE(array);

    EXPECT_TRUE(size != 0);

    functionList.free_array_memory(array, size);
}

TEST_F(FunctionListTest, InitNonexistingArray) {
    size_t size = 100;
    int* array = nullptr;

    EXPECT_TRUE(functionList.init_array_simple(array, size));
}

TEST_F(FunctionListTest, InitExistingArray) {
    size_t size = 0;
    int* array = nullptr;

    array = (int *)functionList.get_array_memory(&size, SIZE_MB);
    EXPECT_TRUE(array);

    EXPECT_FALSE(functionList.init_array_simple(array, size));

    functionList.free_array_memory(array, size);
}

TEST_F(FunctionListTest, InitNonexistingFileArray) {
    size_t size = 0;
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
    size_t size = 0;
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

    array = (int *)functionList.get_array_memory(&size, SIZE_MB);
    EXPECT_TRUE(array);

    EXPECT_FALSE(functionList.init_array(file, array, 15));
    for (int i = 0; i < 15; i++) {
        EXPECT_EQ(array[i], data[i]);
    }

    functionList.free_array_memory(array, size);

    fclose(file);
}

TEST_F(FunctionListTest, InitWrongDataFileArray) {
    size_t size = 0;
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

    array = (int *)functionList.get_array_memory(&size, SIZE_MB);
    EXPECT_TRUE(array);

    EXPECT_TRUE(functionList.init_array(file, array, 5));

    functionList.free_array_memory(array, size);

    fclose(file);
}

TEST_F(FunctionListTest, CalcElementarySumNonexistingArray) {
    size_t size = 0;
    int* array = nullptr;
    int sum = 0;

    EXPECT_TRUE((void *)functionList.count_elementary_sum(array, size, &sum));
}

TEST_F(FunctionListTest, CalcElementarySum) {
    size_t size = 0;
    int* array = nullptr;

    array = (int *)functionList.get_array_memory(&size, SIZE_MB);
    EXPECT_TRUE(array);

    functionList.init_array_simple(array, size);

    int sum = 0;
    functionList.count_elementary_sum(array, size, &sum);

    EXPECT_LT(sum, MODULE);

    functionList.free_array_memory(array, size);
}

TEST_F(FunctionListTest, FreeArrayMemory) {
    size_t size = 0;
    int* array = nullptr;

    array = (int *)functionList.get_array_memory(&size, SIZE_MB);
    EXPECT_TRUE(array);

    EXPECT_FALSE(functionList.free_array_memory(array, size));
}

TEST_F(FunctionListTest, FreeNonexistingArrayMemory) {
    size_t size = 0;
    int* array = nullptr;

    EXPECT_TRUE(functionList.free_array_memory(array, size));
}

// COMPARE LIBRARIES RESULTS SECTION

TEST_F(FunctionListTest, CompareResults) {
    //STATIC INIT
    size_t size_static = 0;
    int * array_static = nullptr;
    int sum_static = 0;

    array_static = get_array_memory(&size_static, SIZE_MB);
    EXPECT_TRUE(array_static);

    init_array_simple(array_static, size_static);
    count_elementary_sum(array_static, size_static, &sum_static);

    free_array_memory(array_static, size_static);

    //DYNAMIC INIT
    size_t size_dynamic = 0;
    int * array_dynamic = nullptr;
    int sum_dynamic = 0;

    array_dynamic = (int *)functionList.get_array_memory(&size_dynamic, SIZE_MB);
    EXPECT_TRUE(array_dynamic);

    functionList.init_array_simple(array_dynamic, size_dynamic);
    functionList.count_elementary_sum(array_dynamic, size_dynamic, &sum_dynamic);

    functionList.free_array_memory(array_dynamic, size_dynamic);

    EXPECT_EQ(sum_dynamic, sum_static);

}