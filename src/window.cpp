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
#include "window.hpp"
#define GETTEXT_PACKAGE "gdar"


Window::Window() :
    a_open(Gtk::Stock::OPEN),
    a_extract(_("Extract")),
    a_info_img(Gtk::Stock::INFO, Gtk::ICON_SIZE_BUTTON),
    img_up(Gtk::Stock::GO_UP, Gtk::ICON_SIZE_BUTTON),
    n_separator(Gtk::ORIENTATION_VERTICAL),
    grey(),
    white()
{
}

Window::~Window() { 
}

void Window::on_show_pwd_dialog() {
    bindtextdomain(GETTEXT_PACKAGE, GDAR_LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);

    PwdSettings p = PwdSettings(*this);
    int result = p.run();
    if (result == Gtk::RESPONSE_OK) {
        dialog_secu_string = p.get_pass();
    }
    dialog_mutex.unlock();
}

