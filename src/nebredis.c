#include <stdlib.h>
#include <string.h>

struct nebredis_server_t {
	char *address;
	unsigned int port;
};

void nebredis_server_destroy(struct nebredis_server_t *server) {
	free(server->address);
	free(server);
}
struct nebredis_server_t *nebredis_server_create(const char *address, unsigned int port) {
	struct nebredis_server_t * server = calloc(1, sizeof(struct nebredis_server_t));
	server->address = strdup(address);
	server->port = port;
	return server;
}
int nebredis_connect(struct nebredis_server_t * server) {
	return 1;
}

int nebredis_disconnect(struct nebredis_server_t * server) {
	return 1;
}
