#include "ui.h"

#include <stdio.h>

user_input* get_add_update(void) {
	struct user_input* input = malloc(sizeof(user_input));

	printf("Enter something like: '122.1.1.1, 32, 10.1.1.0, eth0'\n");
	scanf(
		"%s %s %s %s",
		input->dest,
		input->mask,
		input->oif,
	 	input->gw
	); // quick n dirty for demo

	input->type = ADD_UPDATE;
	return input;
}

user_input* get_del(void) {
	struct user_input* input = malloc(sizeof(user_input));

	printf("Enter something like: 122.1.1.1'\n");
	scanf("%s", input->dest);

	printf("Enter something like: 32'\n");
	scanf("%s", input->mask);

	input->type = DELETE;

	return input;
}

user_input* get_out(void) {
	struct user_input* input = malloc(sizeof(user_input));

	input->type = OUTPUT;

	return input;
}

user_input* ui(void) {
	int selection;

	while ((selection = getchar()) != EOF && selection != '\n');

	switch (selection) {
		case ADD_UPDATE:
			return get_add_update();
			break;
		case DELETE:
			return get_del();
			break;
		case OUTPUT:
			return get_out();
			break;
		default:
			break;
	}
}
