#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define GREEN "\033[1;32m"
#define PINK "\033[1;35m"
#define CYAN "\033[1;36m"
#define DEFAULT "\033[0m\n"

#define DESCRIBE(...)                                        \
	printf(                                                    \
		"\n%sdescribe %s'%s': %s%s",                             \
		CYAN,                                                    \
		PINK,                                                    \
		__func__,                                                \
		__VA_ARGS__,                                             \
		DEFAULT													                         \
	);

#define ASSERT(expr, describe)                               \
	printf("%s\tit %s'%s'%s", CYAN, PINK, describe, DEFAULT);  \
	assert(expr)

#endif /* TEST_UTIL_H */
