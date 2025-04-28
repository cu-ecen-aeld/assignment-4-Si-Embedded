#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
// #define DEBUG_LOG(msg,...)
#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    struct thread_data* thread_func_args = (struct thread_data *) thread_param; 

    DEBUG_LOG ("\n\n\r======== Starting threadfunc ========");

    DEBUG_LOG ("Waiting to lock the mutex.");
    usleep (thread_func_args->wait_to_obtain_ms * 1000);

    if (pthread_mutex_lock (thread_func_args->mutex)) {
        DEBUG_LOG ("Unable to lock mutex.");
        thread_func_args->thread_complete_success = false;
    } 
        
    DEBUG_LOG ("Successfully locked mutex.");
    
    DEBUG_LOG ("Waiting to unlock the mutex.");
    usleep (thread_func_args->wait_to_release_ms * 1000);

    if (pthread_mutex_unlock (thread_func_args->mutex)) {
        /* Not successful. */
        DEBUG_LOG ("Unable to unlock mutex.");
        thread_func_args->thread_complete_success = false;
        return thread_func_args;
    } 

    thread_func_args->thread_complete_success = true;
    DEBUG_LOG ("Successfully unlocked mutex.");
    
    DEBUG_LOG ("Thread finished.\n");
    thread_func_args->thread_complete_success = true;

    return thread_func_args;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,
    int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, 
     * pass thread_data to created thread using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    struct thread_data* thread_args = 
        (struct thread_data*) malloc (sizeof (struct thread_data));

    if (thread_args == NULL) {
        ERROR_LOG ("Failed to allocate memory for thread_args.\n\r");
        return false;
    }

    thread_args->thread = thread;
    thread_args->mutex  = mutex;
    thread_args->wait_to_obtain_ms = wait_to_obtain_ms;
    thread_args->wait_to_release_ms = wait_to_release_ms;
    thread_args->thread_complete_success = false;
    
    if (pthread_create (thread_args->thread, NULL, threadfunc, thread_args)) {
        free (thread_args);
        ERROR_LOG ("Failed to create thread.\n\r");
        return false;
    }

    return true;
}

