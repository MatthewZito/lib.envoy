#include "subscriber.h"
#include "publisher.h"

#include "routing_table.h"

/**
 * @brief Implements a subscriber thread callback;
 * the enclosed subscriber subscribes to a random entry
 *
 * @param arg
 * @return void*
 */
static void* subscriber_thread_routine(void* arg) {
	rt_entry_keys_t keys;

	memset(&keys, 9, sizeof(rt_entry_keys_t));
	strncpy(keys.dest, "122.1.1.1", 16);
	keys.mask = 32;

	// for (int i = 0; ); add more subscriptions
	rt_register(
		publisher_table,
		&keys,
		sizeof(rt_entry_keys_t),
		subscription_callback,
		(uint32_t)arg // this will be the subscriber id
	);

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

	printf("In function %s subscriber %u notified with opcode %s\n",
		__FUNCTION__,
		subscriber_id,
		opcode
	);

}
