#ifndef ENVOY_H
#define ENVOY_H

#include "glthread.h"

#include <stddef.h> /* for size_t */
#include <stdint.h> /* for uint32_t */

#define MAX_NOTIF_KEY_SIZE 64

typedef enum {
	ENVOY_UNKNOWN,
	ENVOY_SUB,
	ENVOY_ADD,
	ENVOY_MOD,
	ENVOY_DEL
} envoy_event_t;

/**
 * @brief Subscriber callback type
 */
typedef void (*envoy_emitter)(void*, size_t);

/**
 * @brief A notifier chain instance
 */
typedef struct notifier_chain {
	char chain_name[64]; /* Name identifier for the chain - really negligible qua functionality */
	glthread_t chain_head; /* Head of the notifier chain instance which this structure represents */
} envoy_t;

/**
 * @brief A notifier chain node instance
 */
typedef struct notifier_chain_node {
	char key[MAX_NOTIF_KEY_SIZE]; /* Subscription key */
	size_t key_size; /* Size of the key subscribed to, where applicable */
	int key_set; /* Bitmask indicating whether the subscriber has subscribed to a specific key */
	envoy_emitter event_fn; /* Event function to invoke when subscription key type events are emitted */
	glthread_t glthread; /* TODO */
} node_t;

#endif /* ENVOY_H */
