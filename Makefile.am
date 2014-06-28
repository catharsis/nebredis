AM_CPPFLAGS += -I$(top_srcdir) -I/usr/local/include/hiredis -I/home/alofgren/repos/naemon/naemon -D_GNU_SOURCE=1 -std=c99 -Wall -Werror

pkglib_LTLIBRARIES = nebredis.la
nebredis_la_LDFLAGS = -module -avoid-version -shared -Wl,-rpath=/usr/local/lib
nebredis_la_LIBADD = -lhiredis -L/usr/local/lib
nebredis_la_SOURCES = src/module.c src/module.h \
					  src/nebredis.c src/nebredis.h \
					  src/command_fmt.c src/command_fmt.h \
					  src/xalloc.c src/xalloc.h
