#ifndef COMMAND_FMT_H
#define COMMAND_FMT_H
#include <naemon/naemon.h>
struct nebredis_hm_command_t;
void nebredis_hm_command_free(struct nebredis_hm_command_t *cmd);
char ** nebredis_hm_command_argv(struct nebredis_hm_command_t *cmd);
int nebredis_hm_command_argc(struct nebredis_hm_command_t *cmd);

struct nebredis_hm_command_t * nebredis_command_format_hm_program(void *program_status);
struct nebredis_hm_command_t * nebredis_command_format_hm_host(void *hst);
struct nebredis_hm_command_t * nebredis_command_format_hm_service(void *svc);
struct nebredis_hm_command_t * nebredis_command_format_hm_contact(void *contact);
struct nebredis_hm_command_t * nebredis_command_format_hm_comment(void *comment);
struct nebredis_hm_command_t * nebredis_command_format_hm_downtime(void *downtime);
#endif

