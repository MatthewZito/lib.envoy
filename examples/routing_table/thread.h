#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

pthread_t* init_thread(int joinable, void*(*callback)(void*), void* ret);

#endif /* THREAD_H */
