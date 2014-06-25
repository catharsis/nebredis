#include <stdio.h>
#include "nebredis.h"

int main(void) {
	struct nebredis_server_t *s;
	redisReply *reply;
	s = nebredis_server_create("127.0.0.1", 6379);
	if (nebredis_connect(s)) {

		fprintf(stderr, "connection failed: %s\n", nebredis_server_errstr(s));
		return -1;
	}
	printf("connected\n");
	if (!(reply = nebredis_command(s, "SET foo 1"))) {
		fprintf(stderr, "command failed: %s\n", nebredis_server_errstr(s));
		nebredis_disconnect(s);
		return -1;
	}
	nebredis_print_reply(reply);

	freeReplyObject(reply);
	if (!(reply = nebredis_command(s, "GET foo"))) {
		fprintf(stderr, "command failed: %s\n", nebredis_server_errstr(s));
		nebredis_disconnect(s);
		return -1;
	}
	nebredis_print_reply(reply);
	freeReplyObject(reply);
	nebredis_disconnect(s);
	printf("disconnected\n");
	nebredis_server_destroy(s);
	return 0;
}
