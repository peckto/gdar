SUBDIRS = src po
dist_doc_DATA = README.md

desktopdir = $(datadir)/applications
desktop_DATA = res/gdar.desktop

INTLTOOL_FILES = po/*.po
		 intltool-extract.in \
                 intltool-merge.in \
                 intltool-update.in

EXTRA_DIST = ${INTLTOOL_FILES} ${desktop_DATA}

DISTCLEANFILES = intltool-extract \
                 intltool-merge \
                 intltool-update \
                 po/.intltool-merge-cache
