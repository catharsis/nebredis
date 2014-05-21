AM_CPPFLAGS += -I$(top_srcdir) -D_GNU_SOURCE=1
pkglib_LTLIBRARIES = nebredis.la
nebredis_la_LDFLAGS = -module -avoid-version -shared
nebredis_la_SOURCES = src/module.c

bin_PROGRAMS = nebredissa
nebredissa_CPPFLAGS = -I/usr/local/include/hiredis
nebredissa_LDFLAGS = -Wl,-rpath=/usr/local/lib 
nebredissa_LDADD = -lhiredis -L/usr/local/lib
nebredissa_SOURCES = src/nebredissa.c src/nebredis.c
