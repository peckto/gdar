/*
    gdar - a graphical user interface to browse and extract dar archives
    Copyright (C) 2017  Tobias Specht

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

#ifndef PWD_DIALOG_HPP
#define PWD_DIALOG_HPP

#include <gtkmm.h>
#include <glibmm/i18n.h>
#include <dar/libdar.hpp>

class PwdSettings : public Gtk::Dialog {
public:
    PwdSettings(Gtk::Window &parent);
    Glib::ustring get_pass();

private:
    Gtk::Box *cont_box;
    Gtk::Box box;

    Gtk::Label p_lable;

    Gtk::Entry p_entry;
    Gtk::Button ok_button, c_button;

};

#endif // PWD_DIALOG_HPP
