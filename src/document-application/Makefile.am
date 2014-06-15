AM_CPPFLAGS =					\
	$(GPATTERN_CFLAGS)			\
	-I$(top_srcdir)				\
	-DG_LOG_DOMAIN=\"GtkDocumentApplication\"\
	-DGPATTERN_COMPILATION

AM_CFLAGS = $(GLIB_WARN_CFLAGS)

lib_LTLIBRARIES = libgtkdocumentapplication-2.0.la

libgtkdocumentapplication_2_0_la_SOURCES =	\
	gtkdocumentapplication.c

libgtkdocumentapplication_2_0_la_LIBADD =		\
	$(GPATTERN_LIBS)

gtkdocumentapplicationincludedir=$(includedir)/gtk-3.0/gtkdocumentapplication/
gtkdocumentapplicationinclude_HEADERS =	\
	gtkdocumentapplication.h
