#ifndef NEBREDIS_H
#define NEBREDIS_H
#include "command_fmt.h"
struct nebredis_server_t;

void nebredis_server_destroy(struct nebredis_server_t * server);
struct nebredis_server_t *nebredis_server_create(const char * address, unsigned int port);
int nebredis_connect(struct nebredis_server_t * server);
void nebredis_disconnect(struct nebredis_server_t * server);
const char *nebredis_server_errstr(struct nebredis_server_t * server);

int nebredis_submit_nebstruct(struct nebredis_server_t * server,
		struct nebredis_hm_command_t * (*command_format_fn)(void *),
		void *data);

enum NebredisError {
	E_PROGRAMMING_ERROR,
	E_COMMAND_ERROR,
	E_CONTEXT_ERROR,
	E_OK
};
enum NebredisError nebredis_command(struct nebredis_server_t * server, const char * format, ...);

/* utilities */
#ifndef __func__
# if __STDC_VERSION__ < 199901L
#  if __GNUC__ >= 2
#   define __func__ __FUNCTION__
#  else
#   define __func__ "<unknown>"
#  endif
# endif
#endif

#define NEBREDIS_ERROR(msg) nm_log(NSLOG_RUNTIME_ERROR, "%s (%s): %s\n", "NebRedis", __func__, msg)
#endif
