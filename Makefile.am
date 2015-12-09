AM_CFLAGS += -std=c99 -Wall -Werror $(HIREDIS_CFLAGS) $(NAEMON_CFLAGS) $(GLIB_CFLAGS)
AM_CPPFLAGS += -I$(top_srcdir) -D_GNU_SOURCE=1
ACLOCAL_AMFLAGS = -I m4
pkglib_LTLIBRARIES = nebredis.la
nebredis_la_LDFLAGS = -module -shared
nebredis_la_LIBADD = $(HIREDIS_LIBS) $(NAEMON_LIBS) $(GLIB_LIBS)
nebredis_la_SOURCES = src/module.c src/module.h \
					  src/nebredis.c src/nebredis.h \
					  src/command_fmt.c src/command_fmt.h \
					  src/xalloc.c src/xalloc.h
