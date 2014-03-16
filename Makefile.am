SUBDIRS = src po
dist_doc_DATA = README.md
INTLTOOL_FILES = po/*.po
		 intltool-extract.in \
                 intltool-merge.in \
                 intltool-update.in

EXTRA_DIST = ${INTLTOOL_FILES}

DISTCLEANFILES = intltool-extract \
                 intltool-merge \
                 intltool-update \
                 po/.intltool-merge-cache
