#include <strings.h>
#include <stdlib.h>
#include "serialize.h"

struct nebredis_serialize_t {
	char *str;
	unsigned int len;
	void *obj;
};

void nebredis_serialize_free(struct nebredis_serialize_t *slz) {
	free(slz->str);
	free(slz);
}

struct nebredis_serialize_t * nebredis_serialize_create(void) {
	struct nebredis_serialize_t *slz = calloc(1, sizeof(struct nebredis_serialize_t));
	slz->len = 0;
	slz->str = NULL;
	slz->obj = NULL;
	return slz;
}

const char * nebredis_serialize_get_string(struct nebredis_serialize_t *slz) {
	return (slz->str);
}

#define UINT(Fname) " " Fname " %u"
#define INT(Fname) " " Fname " %d"
#define STR(Fname) " " Fname " %s"
#define DOUBLE(Fname) " " Fname " %f"

enum NebredisError nebredis_serialize_host(struct nebredis_serialize_t * slz, struct host *hst) {
	if (!slz || !hst) {
		return E_PROGRAMMING_ERROR;
	}
	slz->len = asprintf(&slz->str,
			"hmset host:%s" UINT("id") STR("name") STR("display_name") STR("address") STR("check_command") INT("initial_state") DOUBLE("check_interval") DOUBLE("retry_interval") INT("max_attempts") STR("event_handler") DOUBLE("notification_interval") DOUBLE("first_notification_delay") UINT("notification_options") UINT("hourly_value") STR("notification_period") STR("check_period") INT("flap_detection_enabled") DOUBLE("low_flap_threshold") DOUBLE("high_flap_threshold") INT("flap_detection_options") UINT("stalking_options"),
		hst->name,
		hst->id,
		hst->name,
		hst->display_name,
		hst->address,
		hst->check_command,
		hst->initial_state,
		hst->check_interval,
		hst->retry_interval,
		hst->max_attempts,
		hst->event_handler,
		hst->notification_interval,
		hst->first_notification_delay,
		hst->notification_options,
		hst->hourly_value,
		hst->notification_period,
		hst->check_period,
		hst->flap_detection_enabled,
		hst->low_flap_threshold,
		hst->high_flap_threshold,
		hst->flap_detection_options,
		hst->stalking_options);
	return E_OK;

}
