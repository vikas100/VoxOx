plugindir = $(libdir)/gaim

plugin_LTLIBRARIES = mono.la

mono_la_SOURCES	= \
	mono.c \
	mono-glue.h \
	mono-helper.c \
	mono-helper.h \
	debug-glue.c \
	signal-glue.c \
	blist-glue.c \
	status-glue.c 

mono_la_LDFLAGS  = -module -avoid-version

mono_la_LIBADD = $(MONO_LIBS)

AM_CPPFLAGS = \
	-DVERSION=\"$(VERSION)\" \
	-I$(top_srcdir) \
	-I$(top_srcdir)/libgaim \
	$(DEBUG_CFLAGS) \
	$(PLUGIN_CFLAGS) \
	$(MONO_CFLAGS)
