#ifndef _XALLOC_H
#define _XALLOC_H
void *xmalloc(size_t size);
void *xrealloc(void *ptr, size_t size);
void *xcalloc(size_t count, size_t size);
void *xstrdup(const char *s);
void *xstrndup(const char *s, size_t size);
void xasprintf(char **strp, const char *fmt, ...);
#define xfree(p) free(p); p = NULL;
#endif
