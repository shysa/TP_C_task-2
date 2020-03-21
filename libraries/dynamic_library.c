#include "../include/lib_includes.h"

#include <pthread.h>
#include <sys/sysinfo.h>

// -----------------------------------------------------------------------------------
typedef struct {
    size_t size;
    int offset;
    int * array;
    int thr_sum;
} thr_part_array;

// Подсчет суммы части массива
void* count_part_sum(void* thr_data) {
    thr_part_array* data = (thr_part_array*) thr_data;

    int start_thr_index = data->offset * data->size;
    data->thr_sum = 0;

    for(size_t i = start_thr_index; i < start_thr_index + data->size; i++) {
        data->thr_sum += data->array[i] % MODULE;
        data->thr_sum %= MODULE;
    }

    return NULL;
}

void start_threads(pthread_t* threads, thr_part_array* thr_data, int thr_count) {
    for (size_t i = 0; i < thr_count; i++) {
        pthread_create(&(threads[i]), NULL, count_part_sum, &thr_data[i]);
    }
    //ожидание выполнения всех потоков
    for(int i = 0; i < thr_count; i++) {
        pthread_join(threads[i], NULL);
    }
}

int init_threads(thr_part_array* thr_data, const int * massive, const int thr_count, const size_t massive_size) {
    if (!thr_data || !massive || !thr_count || !massive_size) {
        return -1;
    }

    for (size_t i = 0; i < thr_count; i++) {
        thr_data[i].offset = i;
        thr_data[i].size = massive_size / thr_count;
        thr_data[i].array = (int *) massive;
    }
    return 0;
}

pthread_t* get_memory_thr_handlers(int thr_count) {
    //память под массив идентификаторов потоков
    pthread_t* threads = (pthread_t*) malloc(thr_count * sizeof(pthread_t));
    if (!threads) {
        return NULL;
    }
    return threads;
}
thr_part_array* get_memory_thr_data(int thr_count) {
    //кол-во потоков = кол-во структур потоковых данных
    thr_part_array* thr_data = (thr_part_array*) malloc(thr_count * sizeof(thr_part_array));
    if (!thr_data) {
        return NULL;
    }
    return thr_data;
}

int free_thread_memory(pthread_t* threads) {
    if (!threads) {
        return -1;
    }
    free(threads);
    return 0;
}
int free_thrdata_memory(thr_part_array* thr_data) {
    if (!thr_data) {
        return -1;
    }
    free(thr_data);
    return 0;
}

// -----------------------------------------------------------------------------------

int count_elementary_sum(const int * array, const size_t size, int * sum) {
    if (!array || !size) {
        return -1;
    }
    *sum = 0;

    int thr_count = get_nprocs();
    pthread_t* threads = NULL;
    thr_part_array* thr_data = NULL;

    threads = get_memory_thr_handlers(thr_count);
    if (!threads) {
        return -1;
    }
    thr_data = get_memory_thr_data(thr_count);
    if (!thr_data) {
        return -1;
    }

    if (init_threads(thr_data, array, thr_count, size)) {
        return -1;
    }

    start_threads(threads, thr_data, thr_count);

    for (size_t i = 0; i < thr_count; i++) {
        *sum += thr_data[i].thr_sum % MODULE;
        *sum %= MODULE;
    }

    if (free_thread_memory(threads)) {
        return -1;
    }
    if (free_thrdata_memory(thr_data)) {
        return -1;
    }

    return 0;
}

int* get_array_memory(size_t * size, const size_t size_array_mb) {
    *size = (size_array_mb * 1024 * 1024) / sizeof(int);

    int *massive = (int *) malloc(*size * sizeof(int));
    if (!massive) {
        return NULL;
    }

    return massive;
}

int init_array(int* array, size_t size) {
    if (!array || !size) {
        return -1;
    }

    for (size_t i = 0; i < size; i++) {
        array[i] = 2;
    }
    return 0;
}

int free_array_memory(int* array, size_t size) {
    if (!array || !size) {
        return -1;
    }
    free(array);
    return 0;
}