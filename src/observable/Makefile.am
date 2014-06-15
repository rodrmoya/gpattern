AM_CPPFLAGS =				\
	$(GPATTERN_CFLAGS)		\
	-I$(top_srcdir)			\
	-DG_LOG_DOMAIN=\"GObservable\"	\
	-DGPATTERN_COMPILATION

AM_CFLAGS = $(GLIB_WARN_CFLAGS)

lib_LTLIBRARIES = libgobservable-2.0.la

libgobservable_2_0_la_SOURCES =		\
	gobserver.c			\
	gobservable.c			\
	greactivesubject.c		\
	gpropertyreactivesubject.c

libgobservable_2_0_la_LIBADD =		\
	$(GPATTERN_LIBS)

gobservableincludedir=$(includedir)/glib-2.0/gobservable/
gobservableinclude_HEADERS =		\
	gobserver.h		\
	gobservable.h		\
	greactivesubject.h
