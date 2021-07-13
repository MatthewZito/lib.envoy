#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <stddef.h>

void* subscriber_thread_routine(void* arg);

void* subscription_callback(
	void* arg,
	size_t arg_size,
	envoy_event_t opcode,
	uint32_t subscriber_id
);

#endif /* SUBSCRIBER_H */
