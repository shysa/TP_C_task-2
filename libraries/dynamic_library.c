#include "lib_includes.h"

#include <pthread.h>
#include <sys/sysinfo.h>

// -----------------------------------------------------------------------------------
typedef struct {
    size_t size;
    int offset;
    const int *array;
    long thread_sum;
    int module;
} thread_part_array;

long division_modulo_value(long sum, long array_value, int module) {
    if (!array_value || !module) {
        return INVALID_DATA;
    }
    sum += array_value % module;
    sum %= module;
    return sum;
}

void* count_part_sum(void *thread_data) {
    if (!thread_data) {
        return NULL;
    }

    thread_part_array *data = (thread_part_array *)thread_data;
    unsigned long start_thread_index = data->offset * data->size;
    data->thread_sum = 0;

    for (size_t i = start_thread_index; i < start_thread_index + data->size; i++) {
        data->thread_sum = division_modulo_value(data->thread_sum, data->array[i], data->module);
    }

    pthread_exit(SUCCESS);
}

int start_threads_calc(pthread_t *threads, thread_part_array *thread_data, int thread_count) {
    if (!threads || !thread_data || !thread_count) {
        return INVALID_DATA;
    }

    bool error = false;
    for (size_t i = 0; i < thread_count; i++) {
        if ( pthread_create(&(threads[i]), NULL, count_part_sum, &thread_data[i]) ) {
            error = true;
            break;
        }
    }

    for (int i = 0; i < thread_count; i++) {
        if (pthread_join(threads[i], SUCCESS)) {
            error = true;
            break;
        }
    }
    if (error) {
        return INVALID_DATA;
    }

    return SUCCESS;
}

int init_threads(thread_part_array *thread_data, const int *array, int thread_count, size_t array_size, int module) {
    if (!thread_data || !array || !thread_count || !array_size) {
        return INVALID_DATA;
    }

    for (size_t i = 0; i < thread_count; i++) {
        thread_data[i].offset = i;
        thread_data[i].size = array_size / thread_count;
        thread_data[i].array = (const int *)array;
        thread_data[i].module = module;
    }
    return SUCCESS;
}

pthread_t* get_memory_thread_handlers(int thread_count) {
    if (!thread_count) {
        return NULL;
    }

    pthread_t *threads = (pthread_t *)malloc(thread_count * sizeof(pthread_t));
    if (!threads) {
        return ALLOCATION_ERROR;
    }
    return threads;
}
thread_part_array* get_memory_thread_data(int thread_count) {
    if (!thread_count) {
        return NULL;
    }
    thread_part_array *thread_data = (thread_part_array *)malloc(thread_count * sizeof(thread_part_array));
    if (!thread_data) {
        return ALLOCATION_ERROR;
    }
    return thread_data;
}

int free_thread_memory(pthread_t *threads) {
    if (!threads) {
        return ERROR_FREE_MEMORY;
    }
    free(threads);

    return SUCCESS;
}
int free_thread_data_memory(thread_part_array *thread_data) {
    if (!thread_data) {
        return ERROR_FREE_MEMORY;
    }
    free(thread_data);

    return SUCCESS;
}

// -----------------------------------------------------------------------------------

long count_elementary_sum(const int *array, size_t size, int module) {
    if (!array || !size || !module) {
        return INVALID_DATA;
    }

    int thread_count = get_nprocs();
    pthread_t* threads = NULL;
    thread_part_array* thread_data = NULL;

    threads = get_memory_thread_handlers(thread_count);
    if (!threads) {
        return INVALID_DATA;
    }
    thread_data = get_memory_thread_data(thread_count);
    if (!thread_data) {
        return INVALID_DATA;
    }

    if (init_threads(thread_data, array, thread_count, size, module)) {
        return INVALID_DATA;
    }

    long sum = 0;

    if (start_threads_calc(threads, thread_data, thread_count)) {
        return INVALID_DATA;
    }

    for (size_t i = 0; i < thread_count; i++) {
        sum = division_modulo_value(sum, thread_data[i].thread_sum, module);
        if (sum < 0) {
            return INVALID_DATA;
        }
    }

    if (free_thread_memory(threads)) {
        return ERROR_FREE_MEMORY;
    }
    if (free_thread_data_memory(thread_data)) {
        return ERROR_FREE_MEMORY;
    }

    return sum;
}

int* get_array_memory(size_t size) {
    if (!size) {
        return NULL;
    }

    int *array = (int *)malloc(size * sizeof(int));
    if (!array) {
        return ALLOCATION_ERROR;
    }

    return array;
}

int init_array(FILE *input, int *array, size_t size) {
    if (!array || !size || !input) {
        return INVALID_DATA;
    }

    bool error = false;
    for (size_t i = 0; i < size; i++) {
        if (!fscanf(input, "%d ", &array[i])) {
            error = true;
            free_array_memory(array);
            break;
        }
    }
    if (error) {
        return INVALID_DATA;
    }

    return SUCCESS;
}

int init_array_with_same_default_value(int *array, size_t size) {
    if (!array || !size) {
        return INVALID_DATA;
    }

    for (size_t i = 0; i < size; i++) {
        array[i] = ARRAY_VALUE_FOR_SIMPLE_INIT;
    }

    return SUCCESS;
}

int free_array_memory(int *array) {
    if (!array) {
        return ERROR_FREE_MEMORY;
    }
    free(array);

    return SUCCESS;
}
