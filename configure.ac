#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([gdar], [0.9], [specht.tobias@gmx.de])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/libgdar.cpp])
AC_CONFIG_HEADERS([config.h])
AC_LANG([C++])

AC_CHECK_PROG(MAKE_CHECK,make,yes)
if test x"$MAKE_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Please install make before installing.])
fi

# Internationalization
IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=gdar
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [The domain name to use with gettext.])
ALL_LINGUAS="de"
AM_GLIB_GNU_GETTEXT

PROGRAMNAME_LOCALEDIR=["${datadir}/locale"]
AC_SUBST(PROGRAMNAME_LOCALEDIR)

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
#AC_CHECK_LIB([boost_regex],[boost::regex_match],[],AC_MSG_ERROR([unable to find boost_regex lib]))

#Checks for pkg moduels
libdar="libdar"
AC_ARG_ENABLE([libdar64],[  --enable-libdar64 	link against libdar64],[libdar="libdar64"],[libdar="libdar"])
PKG_CHECK_MODULES([DAR], [${libdar} >= 5.3.0])
PKG_CHECK_MODULES([GTKMM], [gtkmm-3.0])

# Checks for header files.
AC_CHECK_HEADER([boost/regex.hpp],[],[AC_MSG_ERROR([unable to find boost library])])
AC_CHECK_HEADER([gcrypt.h],[],[AC_MSG_ERROR([unable to find gcrypt header])])
AC_CHECK_HEADER([bzlib.h],[],[AC_MSG_ERROR([unable to find bzlib header])])
AC_CHECK_HEADER([lzo/lzo2a.h],[],[AC_MSG_ERROR([unable to find lzo2 header])])
AC_CHECK_HEADER([attr/libattr.h],[],[AC_MSG_ERROR([unable to find libattr header])])
AC_CHECK_HEADER([zlib.h],[],[AC_MSG_ERROR([unable to find zlib header])])

# check special libdar funtion
AC_ARG_ENABLE([date_s],[  --enable-date_s 	use date_s feature of libdar, only available in current git master],
	[AC_DEFINE([GET_CHILDREN_IN_TABLE],[1],[GET_CHILDREN_IN_TABLE])])
AS_VERSION_COMPARE([`pkg-config --modversion ${libdar}`],[5.5.0],
	[],
	AC_DEFINE([LIBDAR_WITH_DATE_AS_TIME],[1],[LIBDAR_WITH_DATE_AS_TIME]),
	AC_DEFINE([LIBDAR_WITH_DATE_AS_TIME],[1],[LIBDAR_WITH_DATE_AS_TIME])
)
AS_VERSION_COMPARE([`pkg-config --modversion ${libdar}`],[5.4.0],
	[],
	AC_DEFINE([GET_CHILDREN_IN_TABLE],[1],[GET_CHILDREN_IN_TABLE]),
	[]
)

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.

#AC_DEFINE([ICON_DIR],[$(datadir)/share/icons/gdar],[ICON_DOR])

AC_CONFIG_FILES([Makefile
                 src/Makefile
		 po/Makefile.in
		])

AC_OUTPUT
