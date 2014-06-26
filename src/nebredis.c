#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <naemon.h>
#include "nebredis.h"
#include "serialize.h"
struct nebredis_server_t {
	char *address;
	unsigned int port;
	redisContext *context;
	redisReply *last_reply;
};

static bool command_failed(redisReply *reply);

static void nebredis_server_free_reply(struct nebredis_server_t *server) {
	if (!server)
		return;
	if (server->last_reply) {
		freeReplyObject(server->last_reply);
	}
	server->last_reply = NULL;
}

void nebredis_server_destroy(struct nebredis_server_t *server) {
	free(server->address);
	redisFree(server->context);
	nebredis_server_free_reply(server);
	free(server);
}


struct nebredis_server_t *nebredis_server_create(const char *address, unsigned int port) {
	struct nebredis_server_t * server = calloc(1, sizeof(struct nebredis_server_t));
	server->address = strdup(address);
	server->port = port;
	server->last_reply = NULL;
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
	else if (command_failed(server->last_reply)) {
		return server->last_reply->str;
	}
	return NULL;
}

void nebredis_disconnect(struct nebredis_server_t * server) {
	return; /*only necessary for asynchronous connections, and we don't have those*/
}

static bool command_failed(redisReply *reply) {
	if (!reply) {
		return true;
	}

	return (bool)(reply->type == REDIS_REPLY_ERROR);
}

enum NebredisError nebredis_command(struct nebredis_server_t * server, const char * format, ...) {
	va_list ap;
	char *command = NULL;
	if (!server || !format) {
		return E_PROGRAMMING_ERROR;
	}
	va_start(ap, format);
	vasprintf(&command, format, ap);
	va_end(ap);
	nebredis_server_free_reply(server);
	server->last_reply = (redisReply *) redisCommand(server->context, command);
	free(command);

	if (server->context->err) {
		return E_CONTEXT_ERROR;
	}
	if (command_failed(server->last_reply)) {
		return E_COMMAND_ERROR;
	}
	return E_OK;
}

int nebredis_submit_host_status_data(struct nebredis_server_t * server, nebstruct_host_status_data *ds) {
	redisReply *reply = NULL;
	struct nebredis_serialize_t * slz = NULL;
	int ret = 0;

	slz = nebredis_serialize_create();
	if (nebredis_serialize_host(slz, (struct host *) ds->object_ptr) != E_OK) {
		NEBREDIS_ERROR("Failed to serialize host struct");
		ret = -1;
	}
	else if ((nebredis_command(server, nebredis_serialize_get_string(slz)) != E_OK)) {
		NEBREDIS_ERROR(nebredis_server_errstr(server));
		ret = -2;
	}
	nebredis_serialize_free(slz);
	return ret;
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
