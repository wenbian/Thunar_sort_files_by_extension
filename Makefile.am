# $Id: Makefile.am 23144 2006-09-13 18:12:13Z benny $

SUBDIRS =								\
	icons								\
	pixmaps								\
	po								\
	po-doc								\
	tdb								\
	thunarx								\
	thunar-vfs							\
	thunar								\
	tests								\
	docs								\
	examples							\
	plugins

distclean-local:
	rm -rf *.spec *.cache *~

rpm: dist
	rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz
	@rm -f $(PACKAGE)-$(VERSION).tar.gz

libexec_SCRIPTS =							\
	ThunarBulkRename						\
	ThunarHelp

ThunarBulkRename: ThunarBulkRename.in Makefile
	rm -f ThunarBulkRename.gen ThunarBulkRename
	sed -e "s,\@bindir\@,$(bindir),g" 				\
		< $(srcdir)/ThunarBulkRename.in				\
		> ThunarBulkRename.gen
	mv ThunarBulkRename.gen ThunarBulkRename

ThunarHelp: ThunarHelp.in Makefile
	rm -f ThunarHelp.gen ThunarHelp
	sed -e "s,\@datadir\@,$(datadir),g" 				\
		< $(srcdir)/ThunarHelp.in				\
		> ThunarHelp.gen
	mv ThunarHelp.gen ThunarHelp

desktopdir = $(datadir)/applications
desktop_in_in_files = 							\
	Thunar.desktop.in.in						\
	Thunar-bulk-rename.desktop.in.in				\
	Thunar-folder-handler.desktop.in.in
desktop_in_files = $(desktop_in_in_files:.desktop.in.in=.desktop.in)
%.desktop.in: %.desktop.in.in
	sed -e "s,\@libexecdir\@,$(libexecdir),g" < $< > $@
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

if HAVE_DBUS
servicedir = $(datadir)/dbus-1/services
service_in_files = org.xfce.FileManager.service.in org.xfce.Thunar.service.in
service_DATA = $(service_in_files:.service.in=.service)
%.service: %.service.in
	sed -e "s,\@bindir\@,$(bindir),g" < $< > $@
endif

EXTRA_DIST =								\
	FAQ								\
	HACKING								\
	ThunarBulkRename.in						\
	Thunar.spec.in							\
	ThunarHelp.in							\
	intltool-extract.in						\
	intltool-merge.in						\
	intltool-update.in						\
	$(desktop_in_in_files)						\
	$(service_in_files)

DISTCLEANFILES =							\
	ThunarBulkRename						\
	Thunar.spec							\
	ThunarHelp							\
	intltool-extract						\
	intltool-merge							\
	intltool-update							\
	$(desktop_in_files)						\
	$(desktop_DATA)							\
	$(service_DATA)

DISTCHECK_CONFIGURE_FLAGS =						\
	--enable-gtk-doc						\
	--enable-xml2po							\
	--enable-xsltproc

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
