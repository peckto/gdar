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
#include "mylibdar.hpp"
#include "gdar_application.hpp"
#include "libgdar.hpp"

using namespace std;

int main(int argc, char **argv) {
    Glib::RefPtr<GdarApplication> application = GdarApplication::create(argc, argv);

    bindtextdomain(GETTEXT_PACKAGE, GDAR_LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);

    return application->run(argc, argv);
}

