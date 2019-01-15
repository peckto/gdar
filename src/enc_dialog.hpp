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

#ifndef ENC_DIALOG_HPP
#define ENC_DIALOG_HPP

#define DEFAULT_BLOCK_SIZE 10240

#include <gtkmm.h>
#include <glibmm/i18n.h>

#include "config.h"
#ifdef LIBDAR5
#include <dar/libdar5.hpp>
#else
#include <dar/libdar.hpp>
#endif

class EncSettings : public Gtk::Dialog {
public:
    EncSettings(Gtk::Window &parent);
    libdar5::secu_string get_pass();
    int get_block_size();
    libdar5::crypto_algo get_crypt_algo();

private:
    Gtk::Box *cont_box;
    Gtk::Box m_box, b_box, p_box;

    Gtk::Label p_lable, c_lable, b_lable;

    Gtk::Grid c_grid;
    Gtk::Entry p_entry;
    Gtk::SpinButton b_spinb;

    Gtk::RadioButtonGroup algo_group;
    Gtk::RadioButton blowfish_check, aes_check, twofish_check, serpent_check, camellia_check;

    Gtk::Button ok_button, c_button;

};

#endif // ENC_DIALOG_HPP
