#include "optional.h"
#include <stdio.h>
#include <stdlib.h>

optional op_empty() {
	return op_wrap(NULL);
}

optional op_wrap(void *data) {
	optional op;
	op.data = data;
	op.has = data != NULL;
	return op;
}

void *op_get(optional op) {
	if(!op.has) {
		fprintf(stderr, "Attempt to get from empty optional.");
		exit(-1);
	}
	return op.data;
}

bool op_has(optional op) {
	return op.has;
}

void *op_if_else(optional op, void *default_val) {
	if(op.has)
		return op.data;
	else
		return default_val;
}
