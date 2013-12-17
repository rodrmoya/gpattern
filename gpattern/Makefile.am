AM_CPPFLAGS =					\
	$(GPATTERN_CFLAGS)			\
	-I$(top_srcdir)				\
	-DG_LOG_DOMAIN=\"GLib-GPattern\"	\
	-DGPATTERN_COMPILATION

AM_CFLAGS = $(GLIB_WARN_CFLAGS)

lib_LTLIBRARIES = libgpattern-2.0.la

libgpattern_2_0_la_SOURCES =		\
	gobserver.c			\
	gobservable.c

libgpattern_2_0_la_LIBADD =		\
	$(GPATTERN_LIBS)

gpatternincludedir=$(includedir)/glib-2.0/gpattern/
gpatterninclude_HEADERS =		\
	gobserver.h		\
	gobservable.h		\
	gpattern.h		\
	gpatterntypes.h
