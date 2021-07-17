#include "libenvoy.h"

#include <stdlib.h> /* for calloc */
#include <memory.h> /* for memcpy */
#include <bsd/bsd.h> /* for strlcpy */

// TODO update to macro
envoy_node_t* glued(glthread_t* glthreadptr) {
	return (envoy_node_t*)((char*)(glthreadptr) - (char *)&(((envoy_node_t*)0)->glthread));
}

/**
 * @brief Allocate and initialize a new Envoy with a given name
 *
 * @param envoy_name
 * @return envoy_t*
 */
envoy_t* envoy_init(char* envoy_name) {
	envoy_t* envoy;

	if ((envoy = calloc(1, sizeof(envoy_t)))) {
		if (strlcpy(envoy->chain_name, envoy_name, sizeof(envoy_name)) >= sizeof(envoy_name)) {
			return NULL;
		}
	} // TODO panic here

	glthread_init(&envoy->chain_head);

	return envoy;
}

/**
 * @brief Register a new node as a subscriber to a given Envoy
 *
 * @param envoy
 * @param node
 */
void envoy_subscribe(envoy_t* envoy, envoy_node_t* node) {
	envoy_node_t* new_node = calloc(1, sizeof(envoy_node_t));

	memcpy(new_node, node, sizeof(envoy_node_t));
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
void envoy_invoke(
	envoy_t* envoy,
	void* arg,
	size_t arg_size,
	char* key,
	size_t key_size,
	envoy_event_t op_code) {

	glthread_t* curr;
	envoy_node_t* node;

	if (IS_GLTHREAD_EMPTY(&envoy->chain_head)) return;

	if (key_size > MAX_NOTIF_KEY_SIZE) return;

	// to check every node's key
	ITERATE_GLTHREAD_BEGIN(&envoy->chain_head, curr) {
		// grab each node
		node = glued(curr);

		// does the node represent a subscriber to a specific key?
		if (key && key_size && node->key_set && (key_size == node->key_size)) {
			// only invoke if the key matches
			if (memcmp(key, node->key, key_size) == 0) {
				node->event_fn(arg, arg_size, op_code, node->subscriber_id);
			}
		} else {
			// the node subscribes to all keys (no key specified)
			node->event_fn(arg, arg_size, op_code, node->subscriber_id);
		}

	} ITERATE_GLTHREAD_END(&envoy->chain_head, curr);
}

/**
 * @brief Purge, delete, and deallocate all nodes of the given envoy
 *
 * @param envoy
 */
void envoy_purge(envoy_t* envoy) {
	glthread_t* curr;
	envoy_node_t* node;

	ITERATE_GLTHREAD_BEGIN(&envoy->chain_head, curr) {

		node = glued(curr);
		glthread_remove(&node->glthread);
		free(node);

	} ITERATE_GLTHREAD_END(&envoy->chain_head, curr);
}
