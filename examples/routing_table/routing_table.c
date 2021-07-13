#include "routing_table.h"

#include "envoy.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief Initialize a routing table
 *
 * @param table
 */
void rt_init(rt_table_t* table) {
	table->head = NULL;
}

/**
 * @brief Add or update the given routing table entry
 *
 * @param table
 * @param dest_ip
 * @param mask
 * @param gw_ip
 * @param oif
 * @return rt_entry_t*
 */
rt_entry_t* rt_add_or_update_entry(
	rt_table_t* table,
	char* dest_ip,
	char mask,
	char* gw_ip,
	char* oif) {

	bool entry_created = false;
	rt_entry_t* head = NULL;
	rt_entry_t* entry;

	if (!(entry = rt_lookup(table, dest_ip, mask))) {
		entry = calloc(1, sizeof(rt_entry_t));

		strncpy(entry->keys.dest, dest_ip, sizeof(entry->keys.dest));
		entry->keys.mask = mask;
		entry_created = true;
	}

	if (gw_ip) strncpy(entry->gw_ip, gw_ip, sizeof(entry->gw_ip));

	if (oif) strncpy(entry->oif, oif, sizeof(entry->oif));

	if (entry_created) {
		head = table->head;
		table->head = entry;
		entry->prev = 0;
		entry->next = head;

		if (head) head->prev = entry;
	}

	return entry;
}

/**
 * @brief Delete the given routing table entry
 *
 * @param table
 * @param dest_ip
 * @param mask
 * @return true
 * @return false
 */
bool rt_del_entry(rt_table_t* table, char* dest_ip, char mask) {
	rt_entry_t* entry = NULL;

	ITER_RT_BEGIN(table, entry) {
		if (strncmp(entry->keys.dest,
			dest_ip, sizeof(entry->keys.dest)) == 0 &&
			entry->keys.mask == mask){

			rt_entry_remove(table, entry);
			free(entry);

			return true;
		}
	} ITER_RT_END(table, curr);

	return false;
}

/**
 * @brief Perform a lookup upon a given routing table
 *
 * @param table
 * @param dest_ip
 * @param mask
 * @return rt_entry_t*
 */
rt_entry_t* rt_lookup(rt_table_t* table, char* dest_ip, char mask) {
	rt_entry_t* entry = NULL;

	ITER_RT_BEGIN(table, entry) {
		if ((strncmp(entry->keys.dest, dest_ip,
			sizeof(entry->keys.dest)) == 0) &&
			entry->keys.mask == mask) {
			return entry;
		}
	} ITER_RT_END(table, entry);

	return NULL;
}

/**
 * @brief Output a given routing table's contents
 *
 * @param table
 */
void rt_out(rt_table_t* table) {
	rt_entry_t *entry = NULL;

	ITER_RT_BEGIN(table, entry) {

		printf(
			"%-20s %-4d %-20s %s\n",
			entry->keys.dest,
			entry->keys.mask,
			entry->gw_ip,
			entry->oif
		);

	} ITER_RT_END(table, entry);
}

/**
 * @brief Remove a given entry from the table
 *
 * @param table
 * @param entry
 */
void rt_entry_remove(rt_table_t* table, rt_entry_t* entry) {
	if (!entry->prev) {
		if (entry->next) {
			entry->next->prev = 0;
			table->head = entry->next;
			entry->next = 0;
			return;
		}

		table->head = 0;
			return;
		}

  if (!entry->next) {
		entry->prev->next = 0;
		entry->prev = 0;
		return;
	}

	entry->prev->next = entry->next;
	entry->next->prev = entry->prev;
	entry->prev = 0;
	entry->next = 0;
}

/**
 * @brief Register a routing table entry subscription
 *
 * @param table
 * @param key
 * @param key_size
 * @param cb
 * @param subscriber_id
 */
void rt_register(
	rt_table_t* table,
	rt_entry_keys_t* key,
	size_t key_size,
	envoy_emitter cb,
	uint32_t subscriber_id) {

	bool entry_created;

	rt_entry_t* entry;
	envoy_node_t node;

	entry_created = false;

	entry = rt_lookup(table, key->dest, key->mask);

	if (!entry) {
		entry = rt_add_or_update_entry(
			table,
			key->dest,
			key->mask,
			0,
			0
		);

		entry_created = true;
	}

	memset(&node, 0, sizeof(envoy_node_t));

	if (key_size <= MAX_NOTIF_KEY_SIZE) return;

	node.event_fn = cb;
	node.subscriber_id = subscriber_id;
	envoy_subscribe(entry->envoy, &node);

	// subscriber subscribes to extant rt entry;
	// call `ENVOY_ADD` opcode
	if (!entry_created) {
		cb(
			entry,
			sizeof(rt_entry_t),
			ENVOY_ADD,
			subscriber_id
		);
	}
}
