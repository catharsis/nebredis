#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <naemon.h>
#include "nebredis.h"
struct nebredis_server_t {
	char *address;
	unsigned int port;
	redisContext *context;
};

void nebredis_server_destroy(struct nebredis_server_t *server) {
	free(server->address);
	redisFree(server->context);
	free(server);
}
struct nebredis_server_t *nebredis_server_create(const char *address, unsigned int port) {
	struct nebredis_server_t * server = calloc(1, sizeof(struct nebredis_server_t));
	server->address = strdup(address);
	server->port = port;
	return server;
}
int nebredis_connect(struct nebredis_server_t * server) {
	server->context = redisConnect(server->address, (int)server->port);
	if (server->context->err) {
		return 1;
	}
	return 0;
}

const char *nebredis_server_errstr(struct nebredis_server_t * server) {
	if (server->context->err) {
		return server->context->errstr;
	}
	return NULL;
}

void nebredis_disconnect(struct nebredis_server_t * server) {
	return; /*only necessary for asynchronous connections, and we don't have those*/
}

redisReply * nebredis_command(struct nebredis_server_t * server, const char * format, ...) {
	va_list ap;
	char *command = NULL;
	redisReply * reply = NULL;
	va_start(ap, format);
	vasprintf(&command, format, ap);
	va_end(ap);
	reply = (redisReply *) redisCommand(server->context, command);
	free(command);
	return reply;
}

int nebredis_submit_host_status_data(struct nebredis_server_t * server, nebstruct_host_status_data *ds) {
	redisReply *reply = NULL;
	if (!(reply = nebredis_command(server, "set host:%s %s", ((struct host *)(ds->object_ptr))->name, "wow"))) {
		NEBREDIS_ERROR(nebredis_server_errstr(server));
		return -1;
	}
	return 0;
}

void nebredis_print_reply(redisReply *reply) {
	redisReply *tmp = NULL;
	switch (reply->type) {
	case REDIS_REPLY_ERROR:
		printf("error: ");
	case REDIS_REPLY_STRING: /* Deliberate  */
	case REDIS_REPLY_STATUS: /* fallthrough */
		printf("%s\n", reply->str);
		break;
	case REDIS_REPLY_INTEGER:
		printf("%lld\n", reply->integer);
		break;
	case REDIS_REPLY_NIL:
		printf("No data (nil)\n");
		break;
	case REDIS_REPLY_ARRAY:
		printf("Array:\n");
		for (int i = 0; i < reply->elements; i++) {
			nebredis_print_reply(reply->element[i]); /*I know, I know.. It's not finished*/
		}
		break;
	default:
		printf("Unhandled reply type: %d, what to do?\n", reply->type);
	}
}

