#ifndef NEBREDIS_H
#define NEBREDIS_H
struct nebredis_server_t;

void nebredis_server_destroy(struct nebredis_server_t * server);
struct nebredis_server_t *nebredis_server_create(const char * address, unsigned int port);
int nebredis_connect(struct nebredis_server_t * server);
void nebredis_disconnect(struct nebredis_server_t * server);

#endif
