#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "nebredis.h"
#include "xalloc.h"
#define log_mem_error() NEBREDIS_ERROR("Failed to allocate memory, aborting.");
#define log_vasprintf_error() NEBREDIS_ERROR("Failed to vasprintf, aborting.");

#define CHECK_AND_RETURN(_ptr)					\
	if (_ptr == NULL) {							\
		log_mem_error();						\
		exit(2);								\
	}											\
	return _ptr;

void *xmalloc(size_t size) {
	void *ptr = malloc(size);
	CHECK_AND_RETURN(ptr);
}

void *xrealloc(void *ptr, size_t size)  {
	void *new_ptr = realloc(ptr, size);
	CHECK_AND_RETURN(new_ptr);
}

void *xcalloc(size_t count, size_t size) {
	void *ptr = calloc(count, size);
	CHECK_AND_RETURN(ptr);
}

void *xstrdup(const char *s) {
	char *str = strdup(s);
	CHECK_AND_RETURN(str);
}

void *xstrndup(const char *s, size_t size) {
	char *str = strndup(s, size);
	CHECK_AND_RETURN(str);
}

void xasprintf(char **strp, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	if (vasprintf(strp, fmt, ap) < 0) {
		log_vasprintf_error();
		exit(2);
	}
	va_end(ap);
}
