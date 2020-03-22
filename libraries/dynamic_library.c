#include "../include/lib_includes.h"

#include <pthread.h>
#include <sys/sysinfo.h>

// -----------------------------------------------------------------------------------
typedef struct {
    size_t size;
    int offset;
    int * array;
    int thread_sum;
} thread_part_array;

// Подсчет суммы части массива
void* count_part_sum(void* thread_data) {
    thread_part_array* data = (thread_part_array*) thread_data;

    int start_thread_index = data->offset * data->size;
    data->thread_sum = 0;

    for (size_t i = start_thread_index; i < start_thread_index + data->size; i++) {
        data->thread_sum += data->array[i] % MODULE;
        data->thread_sum %= MODULE;
    }

    return NULL;
}

void start_threads(pthread_t* threads, thread_part_array* thread_data, int thread_count) {
    for (size_t i = 0; i < thread_count; i++) {
        pthread_create(&(threads[i]), NULL, count_part_sum, &thread_data[i]);
    }
    //ожидание выполнения всех потоков
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }
}

int init_threads(thread_part_array* thread_data, const int * array, int thread_count, size_t array_size) {
    if (!thread_data || !array || !thread_count || !array_size) {
        return INVALID_DATA;
    }

    for (size_t i = 0; i < thread_count; i++) {
        thread_data[i].offset = i;
        thread_data[i].size = array_size / thread_count;
        thread_data[i].array = (int *) array;
    }
    return SUCCESS;
}

pthread_t* get_memory_thread_handlers(int thread_count) {
    //память под массив идентификаторов потоков
    pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
    if (!threads) {
        return ALLOCATION_ERROR;
    }
    return threads;
}
thread_part_array* get_memory_thread_data(int thread_count) {
    //кол-во потоков = кол-во структур потоковых данных
    thread_part_array* thread_data = (thread_part_array*) malloc(thread_count * sizeof(thread_part_array));
    if (!thread_data) {
        return ALLOCATION_ERROR;
    }
    return thread_data;
}

int free_thread_memory(pthread_t* threads) {
    if (!threads) {
        return ERROR_FREE_MEMORY;
    }
    free(threads);

    return SUCCESS;
}
int free_thread_data_memory(thread_part_array* thread_data) {
    if (!thread_data) {
        return ERROR_FREE_MEMORY;
    }
    free(thread_data);

    return SUCCESS;
}

// -----------------------------------------------------------------------------------

int count_elementary_sum(const int * array, const size_t size, int * sum) {
    if (!array || !size) {
        return INVALID_DATA;
    }
    *sum = 0;

    int thread_count = get_nprocs();
    pthread_t* threads = NULL;
    thread_part_array* thread_data = NULL;

    threads = get_memory_thread_handlers(thread_count);
    if (!threads) {
        return (int) ALLOCATION_ERROR;
    }
    thread_data = get_memory_thread_data(thread_count);
    if (!thread_data) {
        return (int) ALLOCATION_ERROR;
    }

    if (init_threads(thread_data, array, thread_count, size)) {
        return INVALID_DATA;
    }

    start_threads(threads, thread_data, thread_count);

    for (size_t i = 0; i < thread_count; i++) {
        *sum += thread_data[i].thread_sum % MODULE;
        *sum %= MODULE;
    }

    if (free_thread_memory(threads)) {
        return ERROR_FREE_MEMORY;
    }
    if (free_thread_data_memory(thread_data)) {
        return ERROR_FREE_MEMORY;
    }

    return SUCCESS;
}

int* get_array_memory(size_t * size, const size_t size_array_mb) {
    *size = (size_array_mb * 1024 * 1024) / sizeof(int);

    int *array = (int *) malloc(*size * sizeof(int));
    if (!array) {
        return ALLOCATION_ERROR;
    }

    return array;
}

int init_array(FILE * input, int* array, size_t size) {
    if (!array || !size || !input) {
        return INVALID_DATA;
    }

    bool error = false;
    for (size_t i = 0; i < size; i++) {
        if (!fscanf(input, "%d ", &array[i])) {
            error = true;
            free_array_memory(array, size);
            break;
        }
    }
    if (error) {
        return INVALID_DATA;
    }

    return SUCCESS;
}

int init_array_simple(int* array, size_t size) {
    if (!array || !size) {
        return INVALID_DATA;
    }

    for (size_t i = 0; i < size; i++) {
        array[i] = ARRAY_VALUE_FOR_SIMPLE_INIT;
    }

    return SUCCESS;
}

int free_array_memory(int* array, size_t size) {
    if (!array || !size) {
        return ERROR_FREE_MEMORY;
    }
    free(array);

    return SUCCESS;
}