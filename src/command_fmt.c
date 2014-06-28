#include <string.h>
#include <stdlib.h>
#include "xalloc.h"
#include "command_fmt.h"

enum NebredisType {
	NEBREDIS_T_STR,
	NEBREDIS_T_ULONG,
	NEBREDIS_T_UINT,
	NEBREDIS_T_INTEGER,
	NEBREDIS_T_DBL,
};

struct nebredis_hm_command_t {
	char **argv;
	size_t argc;
};

static char * nebredis_value_create(const char *field, enum NebredisType t, const void *value) {
	if (!field || !value)
		return NULL;

	char *v = NULL;
	switch(t) {
	case NEBREDIS_T_STR:
		v = xstrdup((char *)value);
		break;
	case NEBREDIS_T_ULONG:
		xasprintf(&v, "%lu", *( unsigned long *)value);
		break;
	case NEBREDIS_T_UINT:
		xasprintf(&v, "%u", *( unsigned int *)value);
		break;
	case NEBREDIS_T_INTEGER:
		xasprintf(&v, "%d", *( int *)value);
		break;
	case NEBREDIS_T_DBL:
		xasprintf(&v, "%f", *( double *)value);
		break;
	}
	return v;
}

char ** nebredis_hm_command_argv(struct nebredis_hm_command_t *cmd) {
	if (!cmd)
		return NULL;
	return cmd->argv;
}

int nebredis_hm_command_argc(struct nebredis_hm_command_t *cmd) {
	if (!cmd)
		return -1;
	return cmd->argc;
}
void nebredis_hm_command_free(struct nebredis_hm_command_t *cmd) {
	int i;
	if (!cmd)
		return;
	for (i = 0; i < cmd->argc; i++) {
		xfree(cmd->argv[i]);
	}
	xfree(cmd->argv);
	xfree(cmd);
}

static void nebredis_hm_command_add_field_value(struct nebredis_hm_command_t *cmd, const char *field, enum NebredisType type, const void *value) {
	if (!value || !cmd)
		return;
	cmd->argv = xrealloc(cmd->argv, (cmd->argc + 2) * sizeof(char *));
	cmd->argv[cmd->argc] = xstrdup(field);
	++(cmd->argc);
	cmd->argv[cmd->argc] = nebredis_value_create(field, type, value);
	++(cmd->argc);
}

static struct nebredis_hm_command_t * nebredis_hm_command_create(const char *key) {
	if (!key)
		return NULL;
	struct nebredis_hm_command_t *cmd = xcalloc(1, sizeof(struct nebredis_hm_command_t));
	cmd->argc = 0;
	cmd->argv = xrealloc(cmd->argv, 2 * sizeof(char *));
	cmd->argv[cmd->argc] = xstrdup("hmset");
	++(cmd->argc);
	cmd->argv[cmd->argc] = xstrdup(key);
	++(cmd->argc);
	return cmd;
}

struct nebredis_hm_command_t * nebredis_command_format_hm_host(struct host *hst) {
	if (!hst) {
		return NULL;
	}
	char *key = NULL;
	struct nebredis_hm_command_t *cmd = NULL;
	xasprintf(&key, "host:%s", hst->name);
	cmd = nebredis_hm_command_create(key); 
	xfree(key);
	nebredis_hm_command_add_field_value(cmd, "id", NEBREDIS_T_UINT, &hst->id);
	nebredis_hm_command_add_field_value(cmd, "name", NEBREDIS_T_STR, hst->name);
	nebredis_hm_command_add_field_value(cmd, "display_name", NEBREDIS_T_STR, hst->display_name);
	nebredis_hm_command_add_field_value(cmd, "alias", NEBREDIS_T_STR, hst->alias);
	nebredis_hm_command_add_field_value(cmd, "address", NEBREDIS_T_STR, hst->address);
	nebredis_hm_command_add_field_value(cmd, "check_command", NEBREDIS_T_STR, hst->check_command);
	nebredis_hm_command_add_field_value(cmd, "initial_state", NEBREDIS_T_INTEGER, &hst->initial_state);
	nebredis_hm_command_add_field_value(cmd, "check_interval", NEBREDIS_T_DBL, &hst->check_interval);
	nebredis_hm_command_add_field_value(cmd, "retry_interval", NEBREDIS_T_DBL, &hst->retry_interval);
	nebredis_hm_command_add_field_value(cmd, "max_attempts", NEBREDIS_T_INTEGER, &hst->max_attempts);
	nebredis_hm_command_add_field_value(cmd, "event_handler", NEBREDIS_T_STR, hst->event_handler);
	nebredis_hm_command_add_field_value(cmd, "notification_interval", NEBREDIS_T_DBL, &hst->notification_interval);
	nebredis_hm_command_add_field_value(cmd, "first_notification_delay", NEBREDIS_T_DBL, &hst->first_notification_delay);
	nebredis_hm_command_add_field_value(cmd, "notification_options", NEBREDIS_T_UINT, &hst->notification_options);
	nebredis_hm_command_add_field_value(cmd, "hourly_value", NEBREDIS_T_UINT, &hst->hourly_value);
	nebredis_hm_command_add_field_value(cmd, "notification_period", NEBREDIS_T_STR, hst->notification_period);
	nebredis_hm_command_add_field_value(cmd, "check_period", NEBREDIS_T_STR, hst->check_period);
	nebredis_hm_command_add_field_value(cmd, "flap_detection_enabled", NEBREDIS_T_INTEGER, &hst->flap_detection_enabled);
	nebredis_hm_command_add_field_value(cmd, "low_flap_threshold", NEBREDIS_T_DBL, &hst->low_flap_threshold);
	nebredis_hm_command_add_field_value(cmd, "high_flap_threshold", NEBREDIS_T_DBL, &hst->high_flap_threshold);
	nebredis_hm_command_add_field_value(cmd, "flap_detection_options", NEBREDIS_T_INTEGER, &hst->flap_detection_options);
	nebredis_hm_command_add_field_value(cmd, "stalking_options", NEBREDIS_T_UINT, &hst->stalking_options);
	nebredis_hm_command_add_field_value(cmd, "check_freshness", NEBREDIS_T_INTEGER, &hst->check_freshness);
	nebredis_hm_command_add_field_value(cmd, "freshness_threshold", NEBREDIS_T_INTEGER, &hst->freshness_threshold);
	nebredis_hm_command_add_field_value(cmd, "process_performance_data", NEBREDIS_T_INTEGER, &hst->process_performance_data);
	nebredis_hm_command_add_field_value(cmd, "checks_enabled", NEBREDIS_T_INTEGER, &hst->checks_enabled);
	nebredis_hm_command_add_field_value(cmd, "check_source", NEBREDIS_T_STR, hst->check_source);
	nebredis_hm_command_add_field_value(cmd, "accept_passive_checks", NEBREDIS_T_INTEGER, &hst->accept_passive_checks);
	nebredis_hm_command_add_field_value(cmd, "event_handler_enabled", NEBREDIS_T_INTEGER, &hst->event_handler_enabled);
	nebredis_hm_command_add_field_value(cmd, "retain_status_information", NEBREDIS_T_INTEGER, &hst->retain_status_information);
	nebredis_hm_command_add_field_value(cmd, "retain_nonstatus_information", NEBREDIS_T_INTEGER, &hst->retain_nonstatus_information);
	nebredis_hm_command_add_field_value(cmd, "obsess", NEBREDIS_T_INTEGER, &hst->obsess);
	nebredis_hm_command_add_field_value(cmd, "notes", NEBREDIS_T_STR, hst->notes);
	nebredis_hm_command_add_field_value(cmd, "notes_url", NEBREDIS_T_STR, hst->notes_url);
	nebredis_hm_command_add_field_value(cmd, "action_url", NEBREDIS_T_STR, hst->action_url);
	nebredis_hm_command_add_field_value(cmd, "icon_image", NEBREDIS_T_STR, hst->icon_image);
	nebredis_hm_command_add_field_value(cmd, "icon_image_alt", NEBREDIS_T_STR, hst->icon_image_alt);
	nebredis_hm_command_add_field_value(cmd, "statusmap_image", NEBREDIS_T_STR, hst->statusmap_image);
	nebredis_hm_command_add_field_value(cmd, "vrml_image", NEBREDIS_T_STR, hst->vrml_image);
	nebredis_hm_command_add_field_value(cmd, "have_2d_coords", NEBREDIS_T_INTEGER, &hst->have_2d_coords);
	nebredis_hm_command_add_field_value(cmd, "x_2d", NEBREDIS_T_INTEGER, &hst->x_2d);
	nebredis_hm_command_add_field_value(cmd, "y_2d", NEBREDIS_T_INTEGER, &hst->y_2d);
	nebredis_hm_command_add_field_value(cmd, "have_3d_coords", NEBREDIS_T_INTEGER, &hst->have_3d_coords);
	nebredis_hm_command_add_field_value(cmd, "x_3d", NEBREDIS_T_DBL, &hst->x_3d);
	nebredis_hm_command_add_field_value(cmd, "y_3d", NEBREDIS_T_DBL, &hst->y_3d);
	nebredis_hm_command_add_field_value(cmd, "z_3d", NEBREDIS_T_DBL, &hst->z_3d);
	nebredis_hm_command_add_field_value(cmd, "should_be_drawn", NEBREDIS_T_INTEGER, &hst->should_be_drawn);
	nebredis_hm_command_add_field_value(cmd, "problem_has_been_acknowledged", NEBREDIS_T_INTEGER, &hst->problem_has_been_acknowledged);
	nebredis_hm_command_add_field_value(cmd, "acknowledgement_type", NEBREDIS_T_INTEGER, &hst->acknowledgement_type);
	nebredis_hm_command_add_field_value(cmd, "check_type", NEBREDIS_T_INTEGER, &hst->check_type);
	nebredis_hm_command_add_field_value(cmd, "current_state", NEBREDIS_T_INTEGER, &hst->current_state);
	nebredis_hm_command_add_field_value(cmd, "last_state", NEBREDIS_T_INTEGER, &hst->last_state);
	nebredis_hm_command_add_field_value(cmd, "last_hard_state", NEBREDIS_T_INTEGER, &hst->last_hard_state);
	nebredis_hm_command_add_field_value(cmd, "plugin_output", NEBREDIS_T_STR, hst->plugin_output);
	nebredis_hm_command_add_field_value(cmd, "long_plugin_output", NEBREDIS_T_STR, hst->long_plugin_output);
	nebredis_hm_command_add_field_value(cmd, "perf_data", NEBREDIS_T_STR, hst->perf_data);
	nebredis_hm_command_add_field_value(cmd, "state_type", NEBREDIS_T_INTEGER, &hst->state_type);
	nebredis_hm_command_add_field_value(cmd, "current_attempt", NEBREDIS_T_INTEGER, &hst->current_attempt);
	nebredis_hm_command_add_field_value(cmd, "current_event_id", NEBREDIS_T_ULONG, &hst->current_event_id);
	nebredis_hm_command_add_field_value(cmd, "last_event_id", NEBREDIS_T_ULONG, &hst->last_event_id);
	nebredis_hm_command_add_field_value(cmd, "current_problem_id", NEBREDIS_T_ULONG, &hst->current_problem_id);
	nebredis_hm_command_add_field_value(cmd, "last_problem_id", NEBREDIS_T_ULONG, &hst->last_problem_id);
	nebredis_hm_command_add_field_value(cmd, "latency", NEBREDIS_T_DBL, &hst->latency);
	nebredis_hm_command_add_field_value(cmd, "execution_time", NEBREDIS_T_DBL, &hst->execution_time);
	nebredis_hm_command_add_field_value(cmd, "is_executing", NEBREDIS_T_INTEGER, &hst->is_executing);
	nebredis_hm_command_add_field_value(cmd, "check_options", NEBREDIS_T_INTEGER, &hst->check_options);
	nebredis_hm_command_add_field_value(cmd, "notifications_enabled", NEBREDIS_T_INTEGER, &hst->notifications_enabled);
	nebredis_hm_command_add_field_value(cmd, "last_notification", NEBREDIS_T_ULONG, &hst->last_notification);
	nebredis_hm_command_add_field_value(cmd, "next_notification", NEBREDIS_T_ULONG, &hst->next_notification);
	nebredis_hm_command_add_field_value(cmd, "next_check", NEBREDIS_T_ULONG, &hst->next_check);
	nebredis_hm_command_add_field_value(cmd, "should_be_scheduled", NEBREDIS_T_INTEGER, &hst->should_be_scheduled);
	nebredis_hm_command_add_field_value(cmd, "last_check", NEBREDIS_T_ULONG, &hst->last_check);
	nebredis_hm_command_add_field_value(cmd, "last_state_change", NEBREDIS_T_ULONG, &hst->last_state_change);
	nebredis_hm_command_add_field_value(cmd, "last_hard_state_change", NEBREDIS_T_ULONG, &hst->last_hard_state_change);
	nebredis_hm_command_add_field_value(cmd, "last_time_up", NEBREDIS_T_ULONG, &hst->last_time_up);
	nebredis_hm_command_add_field_value(cmd, "last_time_down", NEBREDIS_T_ULONG, &hst->last_time_down);
	nebredis_hm_command_add_field_value(cmd, "last_time_unreachable", NEBREDIS_T_ULONG, &hst->last_time_unreachable);
	nebredis_hm_command_add_field_value(cmd, "has_been_checked", NEBREDIS_T_INTEGER, &hst->has_been_checked);
	nebredis_hm_command_add_field_value(cmd, "is_being_freshened", NEBREDIS_T_INTEGER, &hst->is_being_freshened);
	nebredis_hm_command_add_field_value(cmd, "notified_on", NEBREDIS_T_INTEGER, &hst->notified_on);
	nebredis_hm_command_add_field_value(cmd, "current_notification_number", NEBREDIS_T_INTEGER, &hst->current_notification_number);
	nebredis_hm_command_add_field_value(cmd, "no_more_notifications", NEBREDIS_T_INTEGER, &hst->no_more_notifications);
	nebredis_hm_command_add_field_value(cmd, "current_notification_id", NEBREDIS_T_ULONG, &hst->current_notification_id);
	nebredis_hm_command_add_field_value(cmd, "check_flapping_recovery_notification", NEBREDIS_T_INTEGER, &hst->check_flapping_recovery_notification);
	nebredis_hm_command_add_field_value(cmd, "scheduled_downtime_depth", NEBREDIS_T_INTEGER, &hst->scheduled_downtime_depth);
	nebredis_hm_command_add_field_value(cmd, "pending_flex_downtime", NEBREDIS_T_INTEGER, &hst->pending_flex_downtime);
	nebredis_hm_command_add_field_value(cmd, "state_history_index", NEBREDIS_T_INTEGER, &hst->state_history_index);
	nebredis_hm_command_add_field_value(cmd, "last_state_history_update", NEBREDIS_T_ULONG, &hst->last_state_history_update);
	nebredis_hm_command_add_field_value(cmd, "is_flapping", NEBREDIS_T_INTEGER, &hst->is_flapping);
	nebredis_hm_command_add_field_value(cmd, "flapping_comment_id", NEBREDIS_T_ULONG, &hst->flapping_comment_id);
	nebredis_hm_command_add_field_value(cmd, "percent_state_change", NEBREDIS_T_DBL, &hst->percent_state_change);
	nebredis_hm_command_add_field_value(cmd, "total_services", NEBREDIS_T_INTEGER, &hst->total_services);
	nebredis_hm_command_add_field_value(cmd, "total_service_check_interval", NEBREDIS_T_ULONG, &hst->total_service_check_interval);
	nebredis_hm_command_add_field_value(cmd, "modified_attributes", NEBREDIS_T_ULONG, &hst->modified_attributes);

		return cmd;

}

struct nebredis_hm_command_t * nebredis_command_format_hm_service(struct service *svc) {
	if (!svc) {
		return NULL;

	}
	char *key = NULL;

	struct nebredis_hm_command_t *cmd = NULL;

	xasprintf(&key, "host:%s:service:%s", svc->host_name, svc->description);

	cmd = nebredis_hm_command_create(key);
 
	xfree(key);

	nebredis_hm_command_add_field_value(cmd, "id", NEBREDIS_T_UINT, &svc->id);

	nebredis_hm_command_add_field_value(cmd, "host_name", NEBREDIS_T_STR, svc->host_name);

	nebredis_hm_command_add_field_value(cmd, "description", NEBREDIS_T_STR, svc->description);

	nebredis_hm_command_add_field_value(cmd, "display_name", NEBREDIS_T_STR, svc->display_name);
	nebredis_hm_command_add_field_value(cmd, "check_command", NEBREDIS_T_STR, svc->check_command); 
	nebredis_hm_command_add_field_value(cmd, "event_handler", NEBREDIS_T_STR, svc->event_handler); 
	nebredis_hm_command_add_field_value(cmd, "initial_state", NEBREDIS_T_INTEGER, &svc->initial_state); 
	nebredis_hm_command_add_field_value(cmd, "check_interval", NEBREDIS_T_DBL, &svc->check_interval); 
	nebredis_hm_command_add_field_value(cmd, "retry_interval", NEBREDIS_T_DBL, &svc->retry_interval); 
	nebredis_hm_command_add_field_value(cmd, "max_attempts", NEBREDIS_T_INTEGER, &svc->max_attempts); 
	nebredis_hm_command_add_field_value(cmd, "notification_interval", NEBREDIS_T_DBL, &svc->notification_interval); 
	nebredis_hm_command_add_field_value(cmd, "first_notification_delay", NEBREDIS_T_DBL, &svc->first_notification_delay); 
	nebredis_hm_command_add_field_value(cmd, "notification_options", NEBREDIS_T_UINT, &svc->notification_options); 
	nebredis_hm_command_add_field_value(cmd, "stalking_options", NEBREDIS_T_UINT, &svc->stalking_options); 
	nebredis_hm_command_add_field_value(cmd, "hourly_value", NEBREDIS_T_UINT, &svc->hourly_value); 
	nebredis_hm_command_add_field_value(cmd, "is_volatile", NEBREDIS_T_INTEGER, &svc->is_volatile); 
	nebredis_hm_command_add_field_value(cmd, "notification_period", NEBREDIS_T_STR, svc->notification_period); 
	nebredis_hm_command_add_field_value(cmd, "check_period", NEBREDIS_T_STR, svc->check_period); 
	nebredis_hm_command_add_field_value(cmd, "flap_detection_enabled", NEBREDIS_T_INTEGER, &svc->flap_detection_enabled); 
	nebredis_hm_command_add_field_value(cmd, "low_flap_threshold", NEBREDIS_T_DBL, &svc->low_flap_threshold); 
	nebredis_hm_command_add_field_value(cmd, "high_flap_threshold", NEBREDIS_T_DBL, &svc->high_flap_threshold); 
	nebredis_hm_command_add_field_value(cmd, "flap_detection_options", NEBREDIS_T_UINT, &svc->flap_detection_options); 
	nebredis_hm_command_add_field_value(cmd, "process_performance_data", NEBREDIS_T_INTEGER, &svc->process_performance_data); 
	nebredis_hm_command_add_field_value(cmd, "check_freshness", NEBREDIS_T_INTEGER, &svc->check_freshness); 
	nebredis_hm_command_add_field_value(cmd, "freshness_threshold", NEBREDIS_T_INTEGER, &svc->freshness_threshold); 
	nebredis_hm_command_add_field_value(cmd, "accept_passive_checks", NEBREDIS_T_INTEGER, &svc->accept_passive_checks); 
	nebredis_hm_command_add_field_value(cmd, "event_handler_enabled", NEBREDIS_T_INTEGER, &svc->event_handler_enabled); 
	nebredis_hm_command_add_field_value(cmd, "checks_enabled", NEBREDIS_T_INTEGER, &svc->checks_enabled); 
	nebredis_hm_command_add_field_value(cmd, "check_source", NEBREDIS_T_STR, svc->check_source); 
	nebredis_hm_command_add_field_value(cmd, "retain_status_information", NEBREDIS_T_INTEGER, &svc->retain_status_information); 
	nebredis_hm_command_add_field_value(cmd, "retain_nonstatus_information", NEBREDIS_T_INTEGER, &svc->retain_nonstatus_information); 
	nebredis_hm_command_add_field_value(cmd, "notifications_enabled", NEBREDIS_T_INTEGER, &svc->notifications_enabled); 
	nebredis_hm_command_add_field_value(cmd, "obsess", NEBREDIS_T_INTEGER, &svc->obsess); 
	nebredis_hm_command_add_field_value(cmd, "notes", NEBREDIS_T_STR, svc->notes); 
	nebredis_hm_command_add_field_value(cmd, "notes_url", NEBREDIS_T_STR, svc->notes_url); 
	nebredis_hm_command_add_field_value(cmd, "action_url", NEBREDIS_T_STR, svc->action_url); 
	nebredis_hm_command_add_field_value(cmd, "icon_image", NEBREDIS_T_STR, svc->icon_image); 
	nebredis_hm_command_add_field_value(cmd, "icon_image_alt", NEBREDIS_T_STR, svc->icon_image_alt); 
	nebredis_hm_command_add_field_value(cmd, "problem_has_been_acknowledged", NEBREDIS_T_INTEGER, &svc->problem_has_been_acknowledged); 
	nebredis_hm_command_add_field_value(cmd, "acknowledgement_type", NEBREDIS_T_INTEGER, &svc->acknowledgement_type); 
	nebredis_hm_command_add_field_value(cmd, "host_problem_at_last_check", NEBREDIS_T_INTEGER, &svc->host_problem_at_last_check); 
	nebredis_hm_command_add_field_value(cmd, "check_type", NEBREDIS_T_INTEGER, &svc->check_type); 
	nebredis_hm_command_add_field_value(cmd, "current_state", NEBREDIS_T_INTEGER, &svc->current_state); 
	nebredis_hm_command_add_field_value(cmd, "last_state", NEBREDIS_T_INTEGER, &svc->last_state); 
	nebredis_hm_command_add_field_value(cmd, "last_hard_state", NEBREDIS_T_INTEGER, &svc->last_hard_state); 
	nebredis_hm_command_add_field_value(cmd, "plugin_output", NEBREDIS_T_STR, svc->plugin_output); 
	nebredis_hm_command_add_field_value(cmd, "long_plugin_output", NEBREDIS_T_STR, svc->long_plugin_output); 
	nebredis_hm_command_add_field_value(cmd, "perf_data", NEBREDIS_T_STR, svc->perf_data); 
	nebredis_hm_command_add_field_value(cmd, "state_type", NEBREDIS_T_INTEGER, &svc->state_type); 
	nebredis_hm_command_add_field_value(cmd, "next_check", NEBREDIS_T_ULONG, &svc->next_check);;
	nebredis_hm_command_add_field_value(cmd, "should_be_scheduled", NEBREDIS_T_INTEGER, &svc->should_be_scheduled); 
	nebredis_hm_command_add_field_value(cmd, "last_check", NEBREDIS_T_ULONG, &svc->last_check); 
	nebredis_hm_command_add_field_value(cmd, "current_attempt", NEBREDIS_T_INTEGER, &svc->current_attempt); 
	nebredis_hm_command_add_field_value(cmd, "current_event_id", NEBREDIS_T_ULONG, &svc->current_event_id); 
	nebredis_hm_command_add_field_value(cmd, "last_event_id", NEBREDIS_T_ULONG, &svc->last_event_id); 
	nebredis_hm_command_add_field_value(cmd, "last_problem_id", NEBREDIS_T_ULONG, &svc->last_problem_id);
	nebredis_hm_command_add_field_value(cmd, "last_notification", NEBREDIS_T_ULONG, &svc->last_notification);
	nebredis_hm_command_add_field_value(cmd, "next_notification", NEBREDIS_T_ULONG, &svc->next_notification);
	nebredis_hm_command_add_field_value(cmd, "no_more_notifications", NEBREDIS_T_INTEGER, &svc->no_more_notifications);
	nebredis_hm_command_add_field_value(cmd, "check_flapping_recovery_notification", NEBREDIS_T_INTEGER, &svc->check_flapping_recovery_notification);
	nebredis_hm_command_add_field_value(cmd, "last_state_change", NEBREDIS_T_ULONG, &svc->last_state_change);
	nebredis_hm_command_add_field_value(cmd, "last_hard_state_change", NEBREDIS_T_ULONG, &svc->last_hard_state_change);
	nebredis_hm_command_add_field_value(cmd, "last_time_ok", NEBREDIS_T_ULONG, &svc->last_time_ok);
	nebredis_hm_command_add_field_value(cmd, "last_time_warning", NEBREDIS_T_ULONG, &svc->last_time_warning);
	nebredis_hm_command_add_field_value(cmd, "last_time_unknown", NEBREDIS_T_ULONG, &svc->last_time_unknown);
	nebredis_hm_command_add_field_value(cmd, "last_time_critical", NEBREDIS_T_ULONG, &svc->last_time_critical);
	nebredis_hm_command_add_field_value(cmd, "has_been_checked", NEBREDIS_T_INTEGER, &svc->has_been_checked);
	nebredis_hm_command_add_field_value(cmd, "is_being_freshened", NEBREDIS_T_INTEGER, &svc->is_being_freshened);
	nebredis_hm_command_add_field_value(cmd, "notified_on", NEBREDIS_T_UINT, &svc->notified_on);
	nebredis_hm_command_add_field_value(cmd, "current_notification_number", NEBREDIS_T_INTEGER, &svc->current_notification_number);
	nebredis_hm_command_add_field_value(cmd, "current_notification_id", NEBREDIS_T_ULONG, &svc->current_notification_id);
	nebredis_hm_command_add_field_value(cmd, "latency", NEBREDIS_T_DBL, &svc->latency);
	nebredis_hm_command_add_field_value(cmd, "execution_time", NEBREDIS_T_DBL, &svc->execution_time);
	nebredis_hm_command_add_field_value(cmd, "is_executing", NEBREDIS_T_INTEGER, &svc->is_executing);
	nebredis_hm_command_add_field_value(cmd, "check_options", NEBREDIS_T_INTEGER, &svc->check_options);
	nebredis_hm_command_add_field_value(cmd, "scheduled_downtime_depth", NEBREDIS_T_INTEGER, &svc->scheduled_downtime_depth);
	nebredis_hm_command_add_field_value(cmd, "pending_flex_downtime", NEBREDIS_T_INTEGER, &svc->pending_flex_downtime);
	nebredis_hm_command_add_field_value(cmd, "state_history_index", NEBREDIS_T_INTEGER, &svc->state_history_index);
	nebredis_hm_command_add_field_value(cmd, "is_flapping", NEBREDIS_T_INTEGER, &svc->is_flapping);
	nebredis_hm_command_add_field_value(cmd, "flapping_comment_id", NEBREDIS_T_ULONG, &svc->flapping_comment_id);
	nebredis_hm_command_add_field_value(cmd, "percent_state_change", NEBREDIS_T_DBL, &svc->percent_state_change);
	nebredis_hm_command_add_field_value(cmd, "modified_attributes", NEBREDIS_T_ULONG, &svc->modified_attributes);
	nebredis_hm_command_add_field_value(cmd, "event_handler_args",
			NEBREDIS_T_STR, svc->event_handler_args);
	nebredis_hm_command_add_field_value(cmd, "check_command_args", NEBREDIS_T_STR, svc->check_command_args);

	return cmd;

}

