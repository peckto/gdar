/*
    gdar - a graphical user interface to brows and extract dar archives
    Copyright (C) 2014  Tobias Specht

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

    To contact the author: https://gitbub.com/peckto/gdar
*/

#include "enc_dialog.hpp"

EncSettings::EncSettings(Gtk::Window &parent) : 
    Gtk::Dialog(_("Encryption settings"), parent), p_lable(_("Password: ")),
    c_lable(_("Encryption algorithm")), b_lable(_("Block size in bytes")),
    blowfish_check(algo_group,"Blowfish"),aes_check(algo_group,"AES 256"),twofish_check(algo_group,"Twofish 256"),
    serpent_check(algo_group,"Serpent 256"),camellia_check(algo_group,"Camellia 256"),
    m_box(Gtk::ORIENTATION_VERTICAL,0),b_spinb(Gtk::Adjustment::create(DEFAULT_BLOCK_SIZE,1,100000,1,0)),
    p_box(Gtk::ORIENTATION_HORIZONTAL,0),b_box(Gtk::ORIENTATION_HORIZONTAL,0) 
{
    cont_box = get_content_area();
    cont_box->pack_start(m_box);

    m_box.pack_start(p_box);
    m_box.pack_start(c_lable);
    m_box.pack_start(c_grid);
    m_box.pack_start(b_box);

    p_box.pack_start(p_lable);
    p_box.pack_start(p_entry);

    b_box.pack_start(b_lable);
    b_box.pack_start(b_spinb);

    p_entry.set_visibility(false);
    p_entry.signal_activate().connect(sigc::bind<int>(sigc::mem_fun(*this, &EncSettings::response), Gtk::RESPONSE_OK));
    c_grid.attach(blowfish_check,0,0,1,1);
    c_grid.attach(aes_check,0,1,1,1);
    c_grid.attach(twofish_check,0,2,1,1);
    c_grid.attach(serpent_check,1,0,1,1);
    c_grid.attach(camellia_check,1,1,1,1);
    aes_check.set_active();

    add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

   show_all_children();
}

libdar::secu_string EncSettings::get_pass() {
    return libdar::secu_string(p_entry.get_text().c_str(),p_entry.get_text().length());
}

int EncSettings::get_block_size() {
    return (int)b_spinb.get_value();
}

libdar::crypto_algo EncSettings::get_crypt_algo() {
    if (blowfish_check.get_active())
        return libdar::crypto_blowfish;
    if (aes_check.get_active())
        return libdar::crypto_aes256 ;
    if (twofish_check.get_active())
        return libdar::crypto_twofish256;
    if (serpent_check.get_active())
        return libdar::crypto_serpent256;
    if (camellia_check.get_active())
        return libdar::crypto_camellia256;
    return libdar::crypto_none;
}

