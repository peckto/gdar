/* 
    gdar - a graphical user interface to browse and extract dar archives
    Copyright (C) 2021  Tobias Specht

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    To contact the author: https://github.com/peckto/gdar
*/

#include <iostream>
#define BOOST_TEST_MODULE Test Gdar
#include <boost/test/included/unit_test.hpp>
#include "src/mylibdar.hpp"
#include "src/libgdar.hpp"
#include "src/gdar_application.hpp"
#include "src/libgdar.hpp"

using namespace std;
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(test_open_archive, *utf::timeout(2))  {
    Glib::RefPtr<GdarApplication> application = GdarApplication::create(0, NULL);
    GdarOpenWindow window(application.get());
    string path("./foo.1.dar");
    BOOST_TEST(window.parse_dar_file_name(path));
    window.create_mydar();
    BOOST_TEST(window.openDar());

    LIBDAR::entree_stats stats = window.newDar->my_arch->get_stats();
    BOOST_TEST(LIBDAR::deci(stats.num_f).human() == "1");
}

BOOST_AUTO_TEST_CASE(test_parse_dar_file_name) {
    Glib::RefPtr<GdarApplication> application = GdarApplication::create(0, NULL);
    GdarOpenWindow window(application.get());

    string path("/home/bar/backup/foo.1.dar");
    BOOST_TEST(window.parse_dar_file_name(path));
    BOOST_TEST(window.slice == "foo");
    BOOST_TEST(window.path == "/home/bar/backup");

    path = "./foo.1.dar";
    BOOST_TEST(window.parse_dar_file_name(path));
    BOOST_TEST(window.slice == "foo");
    BOOST_TEST(window.path == ".");

    path = "foo.2.dar";
    BOOST_TEST(window.parse_dar_file_name(path));
    BOOST_TEST(window.slice == "foo");
    BOOST_TEST(window.path == ".");

    path = "foo.1";
    BOOST_TEST(!window.parse_dar_file_name(path));
    path = "foo.dar";
    BOOST_TEST(!window.parse_dar_file_name(path));
}
