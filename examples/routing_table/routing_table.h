#ifndef ROUTING_TABLE_H
#define ROUTING_TABLE_H

#include "envoy.h"

#include <stdbool.h>

/*

	The following data structures represent a routing table,
	e.g.

	|    Dest    | Mask |  Oif  |    gw    |
	|------------|------|-------|----------|
	| 122.1.1.1  |  32  | Eth34 | 10.1.1.1 |
	| 122.1.1.2  |  32  | Eth34 | 20.1.1.1 |
	| 122.1.1.3  |  32  | Eth43 | 10.1.1.1 |
	| 122.1.1.4  |  32  | Eth21 | 20.1.2.1 |

*/

#define ADDR_RANGE 100

#define ITER_RT_BEGIN(tableptr, entryptr) {                                                                         \
	rt_entry_t* next;                                                   \
	for((entryptr) = (tableptr)->head; (entryptr); (entryptr) = next) { \
		next = (entryptr)->next;

#define ITER_RT_END(tableptr, entryptr)  }}

/* Structures */

typedef struct routing_table_entry_keys {
	char dest[16];
	char mask;
} rt_entry_keys_t;

typedef struct routing_table_entry {
	rt_entry_keys_t keys;
	char gw_ip[16];
	char oif[32];
	struct routing_table_entry* prev;
	struct routing_table_entry* next;
	envoy_t* envoy;
} rt_entry_t;

typedef struct routing_table {
	rt_entry_t* head;
} rt_table_t;

/* API */

void rt_init(rt_table_t* table);

rt_entry_t* rt_add_or_update_entry(
	rt_table_t* table,
	char* dest_ip,
	char mask,
	char* gw_ip,
	char* oif
);

bool rt_del_entry(rt_table_t *table, char *dest_ip, char mask);

rt_entry_t* rt_lookup(rt_table_t *table, char *dest_ip, char mask);

void rt_out(rt_table_t* table);

void rt_entry_remove(rt_table_t* table, rt_entry_t* entry);

void rt_register(
	rt_table_t* table,
	rt_entry_keys_t* key,
	size_t key_size,
	envoy_emitter cb,
	uint32_t subscriber_id
);

#endif
