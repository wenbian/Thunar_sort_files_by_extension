# $Id: Makefile.am 24675 2007-01-20 20:37:43Z benny $

INCLUDES =								\
	-I$(top_builddir)						\
	-I$(top_builddir)/plugins					\
	-I$(top_srcdir)							\
	-I$(top_srcdir)/plugins						\
	-DBINDIR=\"$(bindir)\"						\
	-DDATADIR=\"$(datadir)\"					\
	-DG_LOG_DOMAIN=\"thunar-uca\"					\
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\"				\
	$(PLATFORM_CPPFLAGS)

extensionsdir = $(libdir)/thunarx-$(THUNAR_VERSION_API)
extensions_LTLIBRARIES =						\
	thunar-uca.la

thunar_uca_la_SOURCES =							\
	thunar-uca-chooser.c						\
	thunar-uca-chooser.h						\
	thunar-uca-context.c						\
	thunar-uca-context.h						\
	thunar-uca-editor.c						\
	thunar-uca-editor.h						\
	thunar-uca-model.c						\
	thunar-uca-model.h						\
	thunar-uca-plugin.c						\
	thunar-uca-private.c						\
	thunar-uca-private.h						\
	thunar-uca-provider.c						\
	thunar-uca-provider.h

thunar_uca_la_CFLAGS =							\
	$(EXO_CFLAGS)							\
	$(PLATFORM_CFLAGS)

thunar_uca_la_LDFLAGS =							\
	-avoid-version							\
	-export-dynamic							\
	-module								\
	$(PLATFORM_LDFLAGS)

defaultsdir = $(sysconfdir)/xdg/Thunar
defaults_in_files = uca.xml.in
defaults_DATA = $(defaults_in_files:.xml.in=.xml)
@INTLTOOL_XML_RULE@

EXTRA_DIST =								\
	README								\
	$(defaults_in_files)

CLEANFILES =								\
	$(defaults_DATA)

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
