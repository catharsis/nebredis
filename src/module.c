#include <stdio.h>
#include <naemon.h>

#include "nebredis.h"
int nebmodule_init(int flags __attribute__ ((__unused__)), char *args, void *handle) {
	return 0;
}

int nebmodule_deinit(int flags __attribute__ ((__unused__)), int reason __attribute__ ((__unused__))) {
	return 0;
}
