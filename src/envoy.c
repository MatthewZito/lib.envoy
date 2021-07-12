#include "envoy.h"

#include <memory.h> /* for memcpy */

/**
 * @brief Register a new node as a subscriber to a given Envoy
 *
 * @param envoy
 * @param node
 */
void subscribe_to_envoy(envoy_t* envoy, node_t* node) {
	node_t* new_node = calloc(1, sizeof(node_t));

	memcpy(new_node, node, sizeof(node_t));
	glthread_init(&new_node->glthread);
	glthread_insert_after(&envoy->chain_head, &new_node->glthread);
}

/**
 * @brief Invoke an envoy, initiating event emission upon all subscribers to `key`
 *
 * @param envoy
 * @param arg
 * @param arg_size
 * @param key
 * @param key_size
 */
void invoke_envoy(
	envoy_t* envoy,
	void* arg,
	size_t arg_size,
	char* key,
	size_t key_size) {

	glthread_t* curr;
	node_t* node;

	if (IS_GLTHREAD_EMPTY(&envoy->chain_head)) return;

	if (key_size > MAX_NOTIF_KEY_SIZE) return;

	// to check every node's key
	ITERATE_GLTHREAD_BEGIN(&envoy->chain_head, curr) {
		// grab each node
		node = glue_to_chain(curr);

		// does the node represent a subscriber to a specific key?
		if (key && key_size && node->key_set && (key_size == node->key_size)) {
			// only invoke if the key matches
			if (memcmp(key, node->key, key_size) == 0) {
				node->event_fn(arg, arg_size);
			}
		} else {
			// the node subscribes to all keys (no key specified)
			node->event_fn(arg, arg_size);
		}

	} ITERATE_GLTHREAD_END(&envoy->chain_head, curr);
}

node_t* glue_to_chain(glthread_t* glthreadptr) {
	return (node_t*)((char*)(glthreadptr) - (char *)&(((node_t*)0)->glthread));
}
