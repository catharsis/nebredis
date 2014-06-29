#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <hiredis/hiredis.h>
#include <naemon/naemon.h>
#include "nebredis.h"
#include "xalloc.h"
#include "command_fmt.h"
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
	xfree(server->address);
	redisFree(server->context);
	nebredis_server_free_reply(server);
	xfree(server);
}


struct nebredis_server_t *nebredis_server_create(const char *address, unsigned int port) {
	struct nebredis_server_t * server = xcalloc(1, sizeof(struct nebredis_server_t));
	server->address = xstrdup(address);
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

enum NebredisError nebredis_command_hmset(struct nebredis_server_t * server, struct nebredis_hm_command_t *cmd) {
	if (!server || !cmd) {
		return E_PROGRAMMING_ERROR;
	}
	nebredis_server_free_reply(server);
	server->last_reply = (redisReply *) redisCommandArgv(server->context, nebredis_hm_command_argc(cmd), (const char **)nebredis_hm_command_argv(cmd), NULL);

	if (server->context->err) {
		return E_CONTEXT_ERROR;
	}
	if (command_failed(server->last_reply)) {
		return E_COMMAND_ERROR;
	}
	return E_OK;
}

int nebredis_submit_nebstruct(struct nebredis_server_t * server, struct nebredis_hm_command_t *(*command_format_fn)(void*), void *data) {
	struct nebredis_hm_command_t * cmd = NULL;
	int ret = 0;
	if ((cmd = command_format_fn(data)) == NULL) {
		NEBREDIS_ERROR("Failed to command-format nebstruct");
		ret = -1;
	}
	else if ((nebredis_command_hmset(server, cmd) != E_OK)) {
		NEBREDIS_ERROR(nebredis_server_errstr(server));
		ret = -2;
	}
	nebredis_hm_command_free(cmd);
	return ret;
}
