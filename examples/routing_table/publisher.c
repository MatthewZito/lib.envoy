#include "publisher.h"

#include <stdlib.h>
#include <stdio.h>

void* publisher_thread_routine(void* arg) {
	int randint = rand() % 10;

	printf(
		"publisher thread updating entries at random beginning with 122.1.1.%d\n",
		randint
	);

	// invoke subscriptions
	for (int i = randint; i < ADDR_RANGE / 10; i++) {
		char buffer[16];
		snprintf(buffer, 16, "122.1.1.%d", i);

		printf("publisher thread updated entry with address %s\n", buffer);

		rt_add_or_update_entry(
			&publisher_table,
			buffer, 32, "10.1.1.0", "eth0"
		);
	}

	// rt_out(&publisher_table);

	return NULL;
}
