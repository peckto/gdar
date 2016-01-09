SUBDIRS = src po
dist_doc_DATA = README

desktopdir = $(datadir)/applications
desktop_DATA = res/gdar.desktop

icondir = $(datadir)/icons/hicolor/256x256/apps
icon_DATA = res/gdar.png

ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4 # fix problem with AX_CXX_COMPILE_STDCXX_11 macro

INTLTOOL_FILES = po/*.po
		 intltool-extract.in \
                 intltool-merge.in \
                 intltool-update.in

EXTRA_DIST = ${INTLTOOL_FILES} ${desktop_DATA} ${icon_DATA}

DISTCLEANFILES = intltool-extract \
                 intltool-merge \
                 intltool-update \
                 po/.intltool-merge-cache
