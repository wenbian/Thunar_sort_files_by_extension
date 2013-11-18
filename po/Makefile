# Makefile for program source directory in GNU NLS utilities package.
# Copyright (C) 1995, 1996, 1997 by Ulrich Drepper <drepper@gnu.ai.mit.edu>
# Copyright (C) 2004-2008 Rodney Dawes <dobey.pwns@gmail.com>
#
# This file may be copied and used freely without restrictions.  It may
# be used in projects which are not available under a GNU Public License,
# but which still want to provide support for the GNU gettext functionality.
#
# - Modified by Owen Taylor <otaylor@redhat.com> to use GETTEXT_PACKAGE
#   instead of PACKAGE and to look for po2tbl in ./ not in intl/
#
# - Modified by jacob berkman <jacob@ximian.com> to install
#   Makefile.in.in and po2tbl.sed.in for use with glib-gettextize
#
# - Modified by Rodney Dawes <dobey.pwns@gmail.com> for use with intltool
#
# We have the following line for use by intltoolize:
# INTLTOOL_MAKEFILE

GETTEXT_PACKAGE = Thunar
PACKAGE = Thunar
VERSION = 1.0.1

SHELL = /bin/sh

srcdir = .
top_srcdir = ..
top_builddir = ..


prefix = /usr/local
exec_prefix = ${prefix}
datadir = ${datarootdir}
datarootdir = ${prefix}/share
libdir = ${exec_prefix}/lib
DATADIRNAME = share
itlocaledir = $(prefix)/$(DATADIRNAME)/locale
subdir = po
install_sh = $(SHELL) /home/em/xyz/Thunar_sort_files_by_extension-master/install-sh
# Automake >= 1.8 provides /bin/mkdir -p.
# Until it can be supposed, use the safe fallback:
mkdir_p = $(install_sh) -d

INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644

GMSGFMT = /usr/bin/msgfmt
MSGFMT = /usr/bin/msgfmt
XGETTEXT_ARGS = --keyword=Q_ --from-code=UTF-8
XGETTEXT = /usr/bin/xgettext $(XGETTEXT_ARGS)
INTLTOOL_UPDATE = /usr/bin/intltool-update
INTLTOOL_EXTRACT = /usr/bin/intltool-extract
MSGMERGE = XGETTEXT_ARGS="$(XGETTEXT_ARGS)" INTLTOOL_EXTRACT=$(INTLTOOL_EXTRACT) srcdir=$(srcdir) $(INTLTOOL_UPDATE) --gettext-package $(GETTEXT_PACKAGE) --dist
GENPOT   = XGETTEXT_ARGS="$(XGETTEXT_ARGS)" INTLTOOL_EXTRACT=$(INTLTOOL_EXTRACT) srcdir=$(srcdir) $(INTLTOOL_UPDATE) --gettext-package $(GETTEXT_PACKAGE) --pot

ALL_LINGUAS = ar be ca cs da de dz el en_GB eo es et eu fi fr gl he hu id it ja ka ko ku lt lv mk nb nl nn pa pl pt_BR pt ro ru sk sq sv tr uk ur zh_CN zh_TW

PO_LINGUAS=$(shell if test -r $(srcdir)/LINGUAS; then grep -v "^\#" $(srcdir)/LINGUAS; else echo "$(ALL_LINGUAS)"; fi)

USER_LINGUAS=$(shell if test -n "$(LINGUAS)"; then LLINGUAS="$(LINGUAS)"; ALINGUAS="$(ALL_LINGUAS)"; for lang in $$LLINGUAS; do if test -n "`grep ^$$lang$$ $(srcdir)/LINGUAS 2>/dev/null`" -o -n "`echo $$ALINGUAS|tr ' ' '\n'|grep ^$$lang$$`"; then printf "$$lang "; fi; done; fi)

USE_LINGUAS=$(shell if test -n "$(USER_LINGUAS)" -o -n "$(LINGUAS)"; then LLINGUAS="$(USER_LINGUAS)"; else if test -n "$(PO_LINGUAS)"; then LLINGUAS="$(PO_LINGUAS)"; else LLINGUAS="$(ALL_LINGUAS)"; fi; fi; for lang in $$LLINGUAS; do printf "$$lang "; done)

POFILES=$(shell LINGUAS="$(PO_LINGUAS)"; for lang in $$LINGUAS; do printf "$$lang.po "; done)

DISTFILES = Makefile.in.in POTFILES.in $(POFILES)
EXTRA_DISTFILES = ChangeLog POTFILES.skip Makevars LINGUAS

POTFILES = \
	../thunar-vfs/thunar-vfs-config.c \
	../thunar-vfs/thunar-vfs-deep-count-job.c \
	../thunar-vfs/thunar-vfs-exec.c \
	../thunar-vfs/thunar-vfs-info.c \
	../thunar-vfs/thunar-vfs-interactive-job.c \
	../thunar-vfs/thunar-vfs-io-jobs.c \
	../thunar-vfs/thunar-vfs-io-local-xfer.c \
	../thunar-vfs/thunar-vfs-io-local.c \
	../thunar-vfs/thunar-vfs-io-ops.c \
	../thunar-vfs/thunar-vfs-io-scandir.c \
	../thunar-vfs/thunar-vfs-io-trash.c \
	../thunar-vfs/thunar-vfs-job.c \
	../thunar-vfs/thunar-vfs-mime-action.c \
	../thunar-vfs/thunar-vfs-mime-application.c \
	../thunar-vfs/thunar-vfs-mime-cache.c \
	../thunar-vfs/thunar-vfs-mime-cleaner.c \
	../thunar-vfs/thunar-vfs-mime-database.c \
	../thunar-vfs/thunar-vfs-mime-handler.c \
	../thunar-vfs/thunar-vfs-mime-info.c \
	../thunar-vfs/thunar-vfs-mime-legacy.c \
	../thunar-vfs/thunar-vfs-mime-parser.c \
	../thunar-vfs/thunar-vfs-mime-provider.c \
	../thunar-vfs/thunar-vfs-mime-sniffer.c \
	../thunar-vfs/thunar-vfs-monitor.c \
	../thunar-vfs/thunar-vfs-os-bsd.c \
	../thunar-vfs/thunar-vfs-os-generic.c \
	../thunar-vfs/thunar-vfs-path.c \
	../thunar-vfs/thunar-vfs-private.c \
	../thunar-vfs/thunar-vfs-simple-job.c \
	../thunar-vfs/thunar-vfs-thumb-jpeg.c \
	../thunar-vfs/thunar-vfs-thumb.c \
	../thunar-vfs/thunar-vfs-transfer-job.c \
	../thunar-vfs/thunar-vfs-user.c \
	../thunar-vfs/thunar-vfs-util.c \
	../thunar-vfs/thunar-vfs-volume-freebsd.c \
	../thunar-vfs/thunar-vfs-volume-hal.c \
	../thunar-vfs/thunar-vfs-volume-manager.c \
	../thunar-vfs/thunar-vfs-volume-none.c \
	../thunar-vfs/thunar-vfs-volume.c \
	../thunar-vfs/thunar-vfs.c \
	../thunar/main.c \
	../thunar/thunar-abstract-dialog.c \
	../thunar/thunar-abstract-icon-view.c \
	../thunar/thunar-application.c \
	../thunar/thunar-chooser-button.c \
	../thunar/thunar-chooser-dialog.c \
	../thunar/thunar-chooser-model.c \
	../thunar/thunar-clipboard-manager.c \
	../thunar/thunar-column-editor.c \
	../thunar/thunar-column-model.c \
	../thunar/thunar-compact-view.c \
	../thunar/thunar-component.c \
	../thunar/thunar-create-dialog.c \
	../thunar/thunar-dbus-client.c \
	../thunar/thunar-dbus-service.c \
	../thunar/thunar-debug.c \
	../thunar/thunar-details-view.c \
	../thunar/thunar-dialogs.c \
	../thunar/thunar-dnd.c \
	../thunar/thunar-emblem-chooser.c \
	../thunar/thunar-enum-types.c \
	../thunar/thunar-file-monitor.c \
	../thunar/thunar-file.c \
	../thunar/thunar-folder.c \
	../thunar/thunar-gdk-extensions.c \
	../thunar/thunar-gobject-extensions.c \
	../thunar/thunar-gtk-extensions.c \
	../thunar/thunar-history-action.c \
	../thunar/thunar-history.c \
	../thunar/thunar-ice.c \
	../thunar/thunar-icon-factory.c \
	../thunar/thunar-icon-renderer.c \
	../thunar/thunar-icon-view.c \
	../thunar/thunar-launcher.c \
	../thunar/thunar-list-model.c \
	../thunar/thunar-location-bar.c \
	../thunar/thunar-location-button.c \
	../thunar/thunar-location-buttons.c \
	../thunar/thunar-location-dialog.c \
	../thunar/thunar-location-entry.c \
	../thunar/thunar-metafile.c \
	../thunar/thunar-navigator.c \
	../thunar/thunar-pango-extensions.c \
	../thunar/thunar-path-entry.c \
	../thunar/thunar-permissions-chooser.c \
	../thunar/thunar-preferences-dialog.c \
	../thunar/thunar-preferences.c \
	../thunar/thunar-progress-dialog.c \
	../thunar/thunar-properties-dialog.c \
	../thunar/thunar-renamer-dialog.c \
	../thunar/thunar-renamer-model.c \
	../thunar/thunar-renamer-pair.c \
	../thunar/thunar-renamer-progress.c \
	../thunar/thunar-sendto-model.c \
	../thunar/thunar-session-client.c \
	../thunar/thunar-settings.desktop.in \
	../thunar/thunar-shortcuts-icon-renderer.c \
	../thunar/thunar-shortcuts-model.c \
	../thunar/thunar-shortcuts-pane.c \
	../thunar/thunar-shortcuts-view.c \
	../thunar/thunar-side-pane.c \
	../thunar/thunar-size-label.c \
	../thunar/thunar-standard-view.c \
	../thunar/thunar-statusbar.c \
	../thunar/thunar-stock.c \
	../thunar/thunar-templates-action.c \
	../thunar/thunar-text-renderer.c \
	../thunar/thunar-throbber.c \
	../thunar/thunar-thumbnail-generator.c \
	../thunar/thunar-trash-action.c \
	../thunar/thunar-tree-model.c \
	../thunar/thunar-tree-pane.c \
	../thunar/thunar-tree-view.c \
	../thunar/thunar-util.c \
	../thunar/thunar-view.c \
	../thunar/thunar-window.c \
	../thunarx/thunarx-property-page.c \
	../thunarx/thunarx-provider-plugin.c \
	../thunarx/thunarx-renamer.c \
	../plugins/thunar-apr/thunar-apr-abstract-page.c \
	../plugins/thunar-apr/thunar-apr-desktop-page.c \
	../plugins/thunar-apr/thunar-apr-image-page.c \
	../plugins/thunar-apr/thunar-apr-plugin.c \
	../plugins/thunar-apr/thunar-apr-provider.c \
	../plugins/thunar-sbr/thunar-sbr-case-renamer.c \
	../plugins/thunar-sbr/thunar-sbr-date-renamer.c \
	../plugins/thunar-sbr/thunar-sbr-enum-types.c \
	../plugins/thunar-sbr/thunar-sbr-insert-renamer.c \
	../plugins/thunar-sbr/thunar-sbr-number-renamer.c \
	../plugins/thunar-sbr/thunar-sbr-plugin.c \
	../plugins/thunar-sbr/thunar-sbr-provider.c \
	../plugins/thunar-sbr/thunar-sbr-remove-renamer.c \
	../plugins/thunar-sbr/thunar-sbr-replace-renamer.c \
	../plugins/thunar-sendto-email/main.c \
	../plugins/thunar-sendto-email/thunar-sendto-email.desktop.in.in \
	../plugins/thunar-tpa/main.c \
	../plugins/thunar-tpa/thunar-tpa-icon.c \
	../plugins/thunar-tpa/thunar-tpa.desktop.in.in \
	../plugins/thunar-uca/thunar-uca-chooser.c \
	../plugins/thunar-uca/thunar-uca-context.c \
	../plugins/thunar-uca/thunar-uca-editor.c \
	../plugins/thunar-uca/thunar-uca-model.c \
	../plugins/thunar-uca/thunar-uca-plugin.c \
	../plugins/thunar-uca/thunar-uca-provider.c \
	../plugins/thunar-uca/uca.xml.in \
	../plugins/thunar-wallpaper/twp-provider.c \
	../Thunar.desktop.in.in \
	../Thunar-bulk-rename.desktop.in.in \
	../Thunar-folder-handler.desktop.in.in

CATALOGS=$(shell LINGUAS="$(USE_LINGUAS)"; for lang in $$LINGUAS; do printf "$$lang.gmo "; done)

.SUFFIXES:
.SUFFIXES: .po .pox .gmo .mo .msg .cat

.po.pox:
	$(MAKE) $(GETTEXT_PACKAGE).pot
	$(MSGMERGE) $< $(GETTEXT_PACKAGE).pot -o $*.pox

.po.mo:
	$(MSGFMT) -o $@ $<

.po.gmo:
	file=`echo $* | sed 's,.*/,,'`.gmo \
	  && rm -f $$file && $(GMSGFMT) -o $$file $<

.po.cat:
	sed -f ../intl/po2msg.sed < $< > $*.msg \
	  && rm -f $@ && gencat $@ $*.msg


all: all-yes

all-yes: $(CATALOGS)
all-no:

$(GETTEXT_PACKAGE).pot: $(POTFILES)
	$(GENPOT)

install: install-data
install-data: install-data-yes
install-data-no: all
install-data-yes: all
	linguas="$(USE_LINGUAS)"; \
	for lang in $$linguas; do \
	  dir=$(DESTDIR)$(itlocaledir)/$$lang/LC_MESSAGES; \
	  $(mkdir_p) $$dir; \
	  if test -r $$lang.gmo; then \
	    $(INSTALL_DATA) $$lang.gmo $$dir/$(GETTEXT_PACKAGE).mo; \
	    echo "installing $$lang.gmo as $$dir/$(GETTEXT_PACKAGE).mo"; \
	  else \
	    $(INSTALL_DATA) $(srcdir)/$$lang.gmo $$dir/$(GETTEXT_PACKAGE).mo; \
	    echo "installing $(srcdir)/$$lang.gmo as" \
		 "$$dir/$(GETTEXT_PACKAGE).mo"; \
	  fi; \
	  if test -r $$lang.gmo.m; then \
	    $(INSTALL_DATA) $$lang.gmo.m $$dir/$(GETTEXT_PACKAGE).mo.m; \
	    echo "installing $$lang.gmo.m as $$dir/$(GETTEXT_PACKAGE).mo.m"; \
	  else \
	    if test -r $(srcdir)/$$lang.gmo.m ; then \
	      $(INSTALL_DATA) $(srcdir)/$$lang.gmo.m \
		$$dir/$(GETTEXT_PACKAGE).mo.m; \
	      echo "installing $(srcdir)/$$lang.gmo.m as" \
		   "$$dir/$(GETTEXT_PACKAGE).mo.m"; \
	    else \
	      true; \
	    fi; \
	  fi; \
	done

# Empty stubs to satisfy archaic automake needs
dvi info tags TAGS ID:

# Define this as empty until I found a useful application.
install-exec installcheck:

uninstall:
	linguas="$(USE_LINGUAS)"; \
	for lang in $$linguas; do \
	  rm -f $(DESTDIR)$(itlocaledir)/$$lang/LC_MESSAGES/$(GETTEXT_PACKAGE).mo; \
	  rm -f $(DESTDIR)$(itlocaledir)/$$lang/LC_MESSAGES/$(GETTEXT_PACKAGE).mo.m; \
	done

check: all $(GETTEXT_PACKAGE).pot
	rm -f missing notexist
	srcdir=$(srcdir) $(INTLTOOL_UPDATE) -m
	if [ -r missing -o -r notexist ]; then \
	  exit 1; \
	fi

mostlyclean:
	rm -f *.pox $(GETTEXT_PACKAGE).pot *.old.po cat-id-tbl.tmp
	rm -f .intltool-merge-cache

clean: mostlyclean

distclean: clean
	rm -f Makefile Makefile.in POTFILES stamp-it
	rm -f *.mo *.msg *.cat *.cat.m *.gmo

maintainer-clean: distclean
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	rm -f Makefile.in.in

distdir = ../$(PACKAGE)-$(VERSION)/$(subdir)
dist distdir: $(DISTFILES)
	dists="$(DISTFILES)"; \
	extra_dists="$(EXTRA_DISTFILES)"; \
	for file in $$extra_dists; do \
	  test -f $(srcdir)/$$file && dists="$$dists $(srcdir)/$$file"; \
	done; \
	for file in $$dists; do \
	  test -f $$file || file="$(srcdir)/$$file"; \
	  ln $$file $(distdir) 2> /dev/null \
	    || cp -p $$file $(distdir); \
	done

update-po: Makefile
	$(MAKE) $(GETTEXT_PACKAGE).pot
	tmpdir=`pwd`; \
	linguas="$(USE_LINGUAS)"; \
	for lang in $$linguas; do \
	  echo "$$lang:"; \
	  result="`$(MSGMERGE) -o $$tmpdir/$$lang.new.po $$lang`"; \
	  if $$result; then \
	    if cmp $(srcdir)/$$lang.po $$tmpdir/$$lang.new.po >/dev/null 2>&1; then \
	      rm -f $$tmpdir/$$lang.new.po; \
            else \
	      if mv -f $$tmpdir/$$lang.new.po $$lang.po; then \
	        :; \
	      else \
	        echo "msgmerge for $$lang.po failed: cannot move $$tmpdir/$$lang.new.po to $$lang.po" 1>&2; \
	        rm -f $$tmpdir/$$lang.new.po; \
	        exit 1; \
	      fi; \
	    fi; \
	  else \
	    echo "msgmerge for $$lang.gmo failed!"; \
	    rm -f $$tmpdir/$$lang.new.po; \
	  fi; \
	done

Makefile POTFILES: stamp-it
	@if test ! -f $@; then \
	  rm -f stamp-it; \
	  $(MAKE) stamp-it; \
	fi

stamp-it: Makefile.in.in $(top_builddir)/config.status POTFILES.in
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/Makefile.in CONFIG_HEADERS= CONFIG_LINKS= \
	       $(SHELL) ./config.status

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
