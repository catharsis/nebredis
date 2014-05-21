#include <stdio.h>
#include <hiredis.h>
#include "nebredis.h"

int main(void) {
	struct nebredis_server_t *s;
	s = nebredis_server_create("127.0.0.1", 6379);
	if (nebredis_connect(s)) {
		fprintf(stderr, "connection failed\n");
		return -1;
	}
	printf("connected\n");
	nebredis_disconnect(s);
	printf("disconnected\n");
	nebredis_server_destroy(s);
	return 0;
}
