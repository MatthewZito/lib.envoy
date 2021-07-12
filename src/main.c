#include <stdlib.h>
#include <stdio.h>

typedef struct flag {
	int flag_on;
} flag_t;

int main(int argc, char* argv[]) {
	flag_t* flag = malloc(sizeof(flag_t));

	flag->flag_on = 0;


	printf("%d\n", 1 << flag->flag_on);
	printf("%d\n", flag->flag_on);
	printf("%d\n", 1 << flag->flag_on);
	printf("%d\n", flag->flag_on);

	return EXIT_SUCCESS;
}
