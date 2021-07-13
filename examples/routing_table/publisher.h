#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "routing_table.h"

#define MAX_SUBSCRIBERS 8

rt_table_t publisher_table;

void* publisher_thread_routine(void* arg);

#endif /* PUBLISHER_H */
