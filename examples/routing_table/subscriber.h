#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <stddef.h>

void* subscriber_thread_routine(void* arg);

void* subscription_callback(void* arg, size_t arg_size);

#endif /* SUBSCRIBER_H */
