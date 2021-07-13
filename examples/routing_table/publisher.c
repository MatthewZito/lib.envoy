#include "publisher.h"

#include "routing_table.h"
#include "ui.h"

#include <stdlib.h>

void* publisher_thread_routine(void* arg) {
	// TODO impl VA ARGS
	rt_add_or_update_entry(
		publisher_table,
		"122.1.1.1", 32, "10.1.1.0", "eth0"
	);
	// printf("\n ")

	rt_add_or_update_entry(
		publisher_table,
		"122.1.1.2", 32, "10.1.1.1", "eth1"
	);

	rt_add_or_update_entry(
		publisher_table,
		"122.1.1.3", 32, "10.1.1.2", "tun0"
	);

	rt_out(publisher_table);
}
