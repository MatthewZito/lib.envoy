#ifndef UI_H
#define UI_H

typedef enum {
	ADD_UPDATE = 1,
	DELETE,
	OUTPUT,
} selections;

typedef struct user_input {
	selections type;
	char dest[16];
	char mask;
	char oif[32];
	char gw[16];
} user_input;

user_input* ui(void);

#endif /* UI_H */
