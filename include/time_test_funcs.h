#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

// ---------------------------
#define SUCCESS 0
#define INVALID_DATA -1
#define ERROR_OPEN_INPUT_TEST_FILE -2
#define ERROR_OPEN_OUTPUT_TEST_FILE -3
// ---------------------------
enum ERROR {
    OPEN_LIBRARY = -10,
    OPEN_LIBRARY_FUNCTION = -20,
    OPEN_FILE = -30,
    ALLOCATION_MEMORY = -40,
    INITIALIZATION_ARRAY = -50,
    COUNT_ELEMENTARY_SUM = -60,
    MEMORY_FREE = -70,
    INVALID_SIZE_PARAMETER = -80
};

typedef struct {
    int size_array;
    long avg_time;
    char *lib_type;
    long module_sum;
} time_statistics;

long mtime();
int output_time_test_data(time_statistics statistics);
int create_text_data(int size);
int set_size_parameter(int argc, const char *argv[], size_t *size);