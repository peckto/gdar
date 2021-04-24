#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_CONFIG_MACRO_DIR([m4])
AC_INIT([gdar], [1.1], [specht.tobias@gmx.de])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/libgdar.cpp])
AC_CONFIG_HEADERS([config.h])
AC_LANG([C++])
AX_CXX_COMPILE_STDCXX([11], [noext], [optional])

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

GDAR_LOCALEDIR=["${datadir}/locale"]
AC_SUBST(GDAR_LOCALEDIR)

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.

#Checks for pkg moduels
libdar="libdar"
AC_ARG_ENABLE([libdar64],[  --enable-libdar64 	link against libdar64],[libdar="libdar64"],[libdar="libdar"])
if test "$libdar" == "libdar64"
then
  AC_DEFINE([LIBDAR_MODE],[64],[LIBDAR_MODE])
fi
PKG_CHECK_MODULES([DAR], [${libdar} >= 5.3.0])
PKG_CHECK_MODULES([GTKMM], [gtkmm-3.0])

# Checks for header files.
AC_CHECK_HEADER([gcrypt.h],[],[AC_MSG_ERROR([unable to find gcrypt header])])
AC_CHECK_HEADER([bzlib.h],[],[AC_MSG_ERROR([unable to find bzlib header])])
AC_CHECK_HEADER([lzo/lzo2a.h],[],[AC_MSG_ERROR([unable to find lzo2 header])])
AC_CHECK_HEADER([attr/libattr.h],[],[AC_MSG_ERROR([unable to find libattr header])])
AC_CHECK_HEADER([zlib.h],[],[AC_MSG_ERROR([unable to find zlib header])])

# check special libdar funtion
AS_VERSION_COMPARE([`pkg-config --modversion ${libdar}`],[5.5.0],
	[libdar_with_date_as_time="no"],
	[libdar_with_date_as_time="no"],
	[libdar_with_date_as_time="yes"]
)
AS_VERSION_COMPARE([`pkg-config --modversion ${libdar}`],[5.4.0],
	[get_children_in_table="no"],
	[get_children_in_table="yes"],
	[get_children_in_table="yes"]
)
AS_VERSION_COMPARE([`pkg-config --modversion ${libdar}`],[6.0.0],
        [use_libdar_api5="no"],
        [use_libdar_api5="yes"],
        [use_libdar_api5="yes"]
)

AS_VERSION_COMPARE([`pkg-config --modversion gtkmm-3.0`],[3.6.0],
	[set_always_show_image="no"],
	[set_always_show_image="yes"],
	[set_always_show_image="yes"]
)

if test "$libdar_with_date_as_time" = "yes"
then
    AC_DEFINE([LIBDAR_WITH_DATE_AS_TIME],[1],[LIBDAR_WITH_DATE_AS_TIME])
fi
if test "$get_children_in_table" = "yes"
then
    AC_DEFINE([GET_CHILDREN_IN_TABLE],[1],[GET_CHILDREN_IN_TABLE])
fi
if test "$set_always_show_image" = "yes"
then
    AC_DEFINE([SET_ALWAYS_SHOW_IMAGE],[1],[SET_ALWAYS_SHOW_IMAGE])
fi
if test "$use_libdar_api5" = "yes"
then
    AC_DEFINE([LIBDAR5],[1],[LIBDAR5])
fi


# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.

#AC_DEFINE([ICON_DIR],[$(datadir)/share/icons/gdar],[ICON_DOR])

AC_CONFIG_FILES([Makefile
                 src/Makefile
		 po/Makefile.in
		])

AC_OUTPUT

echo
echo "Enabled features:"
echo "    libdar children in table: $get_children_in_table"
echo "    libdar support date_s: $libdar_with_date_as_time"
echo "    use libdar API v5: $use_libdar_api5"
echo "    gtk always show image: $set_always_show_image"
