#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void* threadfunc(void* thread_param)
{
    struct thread_data* thread_func_data = (struct thread_data *) thread_param;

    usleep(thread_func_data->wait_to_obtain_ms * 1000);

    if (pthread_mutex_lock(thread_func_data->mutex) != 0) {
        thread_func_data->thread_complete_success = false;
        return thread_param;
    }

    usleep(thread_func_data->wait_to_release_ms * 1000);

    if (pthread_mutex_unlock(thread_func_data->mutex) != 0) {
        thread_func_data->thread_complete_success = false;
        return thread_param;
    }

    thread_func_data->thread_complete_success = true;
    return thread_param;
}

bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex, int wait_to_obtain_ms, int wait_to_release_ms)
{
    struct thread_data *data = (struct thread_data*) malloc(sizeof(struct thread_data));
    if (data == NULL) {
        return false;
    }

    data->mutex = mutex;
    data->wait_to_obtain_ms = wait_to_obtain_ms;
    data->wait_to_release_ms = wait_to_release_ms;
    data->thread_complete_success = false;

    if (pthread_create(thread, NULL, threadfunc, (void*)data) != 0) {
        free(data);
        return false;
    }

    return true;
}
