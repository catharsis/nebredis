#ifndef SERIALIZE_H
#define SERIALIZE_H
#include <naemon.h>
#include "nebredis.h"
struct nebredis_serialize_t;
void nebredis_serialize_free(struct nebredis_serialize_t *slz);
struct nebredis_serialize_t * nebredis_serialize_create(void);
const char * nebredis_serialize_get_string(struct nebredis_serialize_t *slz);

enum NebredisError nebredis_serialize_host(struct nebredis_serialize_t * fmt, struct host *hst);
#endif

