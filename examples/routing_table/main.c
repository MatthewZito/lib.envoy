#include "publisher.h"
#include "routing_table.h"
#include "subscriber.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> /* for sleep */

/* main thread */

int main(int argc, char* argv[]) {
	srand(time(NULL));

	rt_init(&publisher_table);

	// seed routing table
	for (int i = 0; i < ADDR_RANGE; i++) {
		char buffer[11];
		snprintf(buffer, 11, "122.1.1.%d", i);

		rt_add_or_update_entry(
			&publisher_table,
			buffer, 32, "10.1.1.0", "eth0"
		);
	}

	pthread_t threads[MAX_SUBSCRIBERS];

	int i;
	// initialize the subscriber threads
	for (i = 0; i < MAX_SUBSCRIBERS; i++) {
		pthread_t thread;

		if (pthread_create(&thread, NULL, subscriber_thread_routine, (void*)&i /* subscriber id */) != 0) {
			perror("thread initialization");
			continue;
		}

		threads[i] = thread;
		// arbitrary pause for demo
		sleep(1);
	}

	// initialize the publisher thread
	pthread_t publisher_thread;

	if (pthread_create(&publisher_thread, NULL, publisher_thread_routine, NULL) != 0) {
		perror("thread initialization");
		return EXIT_FAILURE;
	}

	printf("\npublisher thread initialized\n");

	pthread_join(publisher_thread, NULL);

	for (; i > 0; i--) {
		pthread_join(threads[i], NULL);
	}

	pthread_exit(0);

	return EXIT_SUCCESS;
}
