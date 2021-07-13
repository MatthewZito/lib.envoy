
#include "publisher.h"
#include "routing_table.h"
#include "subscriber.h"

#include <string.h>
#include <stdio.h> /* for printf */
#include <stdlib.h> /* for rand */

/**
 * @brief Implements a subscriber thread callback;
 * the enclosed subscriber subscribes to a random entry
 *
 * @param arg
 * @return void*
 */
void* subscriber_thread_routine(void* arg) {
	rt_entry_keys_t keys;

	// generate subscriptions
	int randint = rand() % 10;
	for (int i = randint; i < ADDR_RANGE / 10; i++) {
		char buffer[16];
		snprintf(buffer, 16, "122.1.1.%d", i);

		memset(&keys, 0, sizeof(rt_entry_keys_t));
		strncpy(keys.dest, buffer, 16);

		keys.mask = 32;

		rt_register(
			&publisher_table,
			&keys,
			sizeof(rt_entry_keys_t),
			subscription_callback,
			(uint32_t)arg // this will be the subscriber id
		);

		printf("subscriber thread subscribed to entry with address %s\n", buffer);
	}

	return NULL;
}

/**
 * @brief Subscription callback to be invoked by envoy notifier
 *
 * @param arg
 * @param arg_size
 * @param opcode
 * @param subscriber_id
 * @return void*
 */
void* subscription_callback(
	void* arg,
	size_t arg_size,
	envoy_event_t opcode,
	uint32_t subscriber_id) {

	rt_entry_t* entry = (rt_entry_t*)arg;

	printf("subscriber %u notified with opcode %d for addr %s\n",
		subscriber_id,
		opcode,
		(char*)entry
	);

	return NULL;
}
