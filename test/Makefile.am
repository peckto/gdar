TESTS = test_gdar
check_PROGRAMS = test_gdar
test_gdar_SOURCES = test_gdar.cpp
test_gdar_CPPFLAGS = $(GTKMM_CFLAGS) $(DAR_CFLAGS) -DGDAR_LOCALEDIR=\"${GDAR_LOCALEDIR}\"
test_gdar_LDADD = $(top_builddir)/src/gdar_application.o $(top_builddir)/src/enc_dialog.o $(top_builddir)/src/myuser_interaction.o $(top_builddir)/src/file.o $(top_builddir)/src/pwd_dialog.o $(top_builddir)/src/window.o $(top_builddir)/src/libgdar.o $(top_builddir)/src/mylibdar.o $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(GTKMM_LIBS) $(DAR_LIBS)
