#include <stdio.h>

#include "nebredis.h"

void *modhandle;

struct nebredis_server_t *g_server;
NEB_API_VERSION(4)
/* Neb Callbacks */
int nh_host_status_data(int cb, void *data) {
	nebstruct_host_status_data *ds = (nebstruct_host_status_data *)data;
	return nebredis_submit_host_status_data(g_server, ds);
}

int nh_service_status_data(int cb, void *data) {
	nebstruct_service_status_data *ds = (nebstruct_service_status_data *)data;
	return nebredis_submit_service_status_data(g_server, ds);
}

int nebmodule_init(int flags __attribute__ ((__unused__)), char *args, void *handle) {
	g_server = nebredis_server_create("127.0.0.1", 6379);
	if (nebredis_connect(g_server)) {
		NEBREDIS_ERROR(nebredis_server_errstr(g_server));
		return 1;
	}
	modhandle = handle;
	neb_register_callback(NEBCALLBACK_HOST_STATUS_DATA, modhandle, 0, nh_host_status_data);
	neb_register_callback(NEBCALLBACK_SERVICE_STATUS_DATA, modhandle, 0, nh_service_status_data);
	return 0;
}

int nebmodule_deinit(int flags __attribute__ ((__unused__)), int reason __attribute__ ((__unused__))) {
	nebredis_disconnect(g_server);
	nebredis_server_destroy(g_server);
	return 0;
}
