#include "thread.h"

pthread_t* init_thread(int joinable, void*(*callback)(void*), void* ret) {
	pthread_attr_t attr;
	pthread_t thread;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(
		&attr,
		joinable == 0 ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE
	);

	pthread_create(&thread, &attr, callback, ret);
}
