#include "publisher.h"
#include "ui.h"
#include "routing_table.h"
#include "subscriber.h"
#include "thread.h"

#include <stdlib.h>

/* main thread */

int main(int argc, char* argv[]) {
	pthread_t threads[MAX_SUBSCRIBERS + 1];

	rt_init(&publisher_table);

	int i;
	// initialize the subscriber threads
	for (i = 0; i < MAX_SUBSCRIBERS; i++) {
		threads[i] = init_thread(
			1,
			subscriber_thread_routine,
			i /* subscriber id */
		);

		// arbitrary pause for demo
		sleep(1);
	}

	// initialize the publisher thread
	pthread_t publisher_thread = init_thread(0, publisher_thread_routine, NULL);

	// generate the routing table
	while (1) {
		puts("Publisher menu");
		puts("(1) Add/Update routing table entry");
		puts("(2) Delete routing table entry");
		puts("(3) Output routing table");
		puts("(4) Done");
		user_input* selection = ui();

		switch (selection->type) {
			case OUTPUT:
				rt_out(publisher_table);
				break;
			case ADD_UPDATE:
				rt_add_or_update_entry(
					publisher_table,
					selection->dest,
					selection->mask,
					selection->gw,
					selection->oif
				);
				break;
			case DELETE:
				rt_del_entry(
					publisher_table,
					selection->dest,
					selection->mask
				);
				break;
			case 4: break;break;
			default: continue;
		}
	}

	for (; i > 0; i--) {
		pthread_join(threads[i], NULL);
	}

	pthread_exit(0);
	return EXIT_SUCCESS;
}
