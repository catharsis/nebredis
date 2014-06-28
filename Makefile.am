AM_CPPFLAGS += -I$(top_srcdir) -D_GNU_SOURCE=1 -std=c99 -Wall -Werror

pkglib_LTLIBRARIES = nebredis.la
nebredis_la_LDFLAGS = -module -shared
nebredis_la_LIBADD = -lhiredis
nebredis_la_SOURCES = src/module.c src/module.h \
					  src/nebredis.c src/nebredis.h \
					  src/command_fmt.c src/command_fmt.h \
					  src/xalloc.c src/xalloc.h
