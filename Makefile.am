AM_CPPFLAGS += -I$(top_srcdir) -I/usr/local/include/hiredis -I/home/alofgren/repos/naemon/naemon -D_GNU_SOURCE=1 -std=c99

pkglib_LTLIBRARIES = nebredis.la
nebredis_la_LDFLAGS = -module -avoid-version -shared -Wl,-rpath=/usr/local/lib
nebredis_la_LIBADD = -lhiredis -L/usr/local/lib
nebredis_la_SOURCES = src/module.c src/nebredis.c

#bin_PROGRAMS = nebredissa
#nebredissa_LDFLAGS = -Wl,-rpath=/usr/local/lib
#nebredissa_LDADD = -lhiredis -L/usr/local/lib
#nebredissa_SOURCES = src/nebredissa.c src/nebredis.c
