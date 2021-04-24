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

#include "pwd_dialog.hpp"

PwdSettings::PwdSettings(Gtk::Window &parent) : 
    Gtk::Dialog(_("Password"), parent),
    box(Gtk::ORIENTATION_VERTICAL,0),
    p_lable(_("Please enter password for archive:"))
{
    cont_box = get_content_area();
    cont_box->pack_start(box);

    box.pack_start(p_lable);
    box.pack_start(p_entry);

    p_entry.set_visibility(false);
    p_entry.signal_activate().connect(sigc::bind<int>(sigc::mem_fun(*this, &PwdSettings::response), Gtk::RESPONSE_OK));

    add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

   show_all_children();
}

Glib::ustring PwdSettings::get_pass() {
    return p_entry.get_text();
}

