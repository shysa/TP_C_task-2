#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ACCURACY 10
#define DEFAULT_ARRAY_VALUE_FOR_TEST 3
#define TEST_MEMORY_STEP 2
#define DEFAULT_INPUT_TXT "data_test_input.txt"
// ---------------------------
#define SUCCESS 0
#define INVALID_DATA -1
#define ERROR_OPEN_INPUT_TEST_FILE -2
#define ERROR_OPEN_OUTPUT_TEST_FILE -3
// ---------------------------
// Ошибки в dynamic_test/static_test:
#define ERROR_OPEN_LIBRARY -10
#define ERROR_OPEN_LIBRARY_FUNCTION -20
#define ERROR_OPEN_FILE -30
#define ERROR_ALLOCATION_MEMORY -40
#define ERROR_INITIALIZATION_ARRAY -50
#define ERROR_COUNT_ELEMENTARY_SUM -60
#define ERROR_MEMORY_FREE -70
// ---------------------------

typedef struct {
    int size_array_mb;
    long avg_time;
    char lib_type[15];
    int module_sum;
} time_statistics;

long mtime();
int output_time_test_data(time_statistics statistics);
int create_text_data(int size);
