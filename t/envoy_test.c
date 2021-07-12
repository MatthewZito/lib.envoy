#include "envoy.h"

#include <stdlib.h>

/**
 * Lifecycle
 */

int run_test(envoy_t* (*setup)(void), void (*teardown)(envoy_t*), envoy_t* (*test)(envoy_t*)) {
	teardown(test(setup()));
}

envoy_t* setup(void) {
	return envoy_init("test_envoy");
}

void teardown(envoy_t* envoy) {
	// csll_iterate(ll, free);
	// free(ll);
}

envoy_t* test_notify_keyholder(envoy_t* envoy) {

	return envoy;
}

envoy_t* test_notify_keyholders(envoy_t* envoy);

envoy_t* test_notify_subscriber_to_all(envoy_t* envoy);

envoy_t* test_multi_chain(envoy_t* envoy);

envoy_t* test_subscribe(envoy_t* envoy);

envoy_t* test_unsubscribe(envoy_t* envoy);

int main() {

	return EXIT_SUCCESS;
}
