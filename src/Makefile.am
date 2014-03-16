BOOST_REGEX_LIBS = -lboost_regex

AM_CPPFLAGS = $(GTKMM_CFLAGS) $(DAR_CFLAGS) -DPROGRAMNAME_LOCALEDIR=\"${PROGRAMNAME_LOCALEDIR}\"
bin_PROGRAMS = gdar
gdar_SOURCES = gdar.cpp mylibdar.cpp mylibdar.hpp libgdar.cpp libgdar.hpp
gdar_LDADD = $(GTKMM_LIBS) $(DAR_LIBS) $(BOOST_REGEX_LIBS)
