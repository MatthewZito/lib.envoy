#ifndef PUBLISHER_H
#define PUBLISHER_H

#define MAX_SUBSCRIBERS 100

static const rt_table_t* publisher_table;

void* publisher_thread_routine(void* arg);

#endif /* PUBLISHER_H */
