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


#ifndef MYLIBDAR_HPP
#define MYLIBDAR_HPP

#ifdef LIBDAR5
#include <dar/libdar5.hpp>
#else
#include <dar/libdar.hpp>
#endif
#include <dar/deci.hpp>
#include <libintl.h>
#include "myuser_interaction.hpp"
#include "enc_dialog.hpp"
#include "window.hpp"
#include "config.h"

#define DEFAULT_CRYPTO_SIZE 10240


class Mydar {
public:
    libdar::archive *my_arch;
    Dialog dialog;
    Dialog_custom_listing dialog_custom_listing;
    libdar::U_16 exception;
    std::string except_msg;
    std::string stats_total;
    std::string path;
    std::string slice;

    Mydar(Window *parentWindow);
    Mydar(Window *parentWindow, std::string path, std::string slice);
    ~Mydar();
    // init libdar
    int init();
    // open dar archive
    int open(std::string path, std::string slice, libdar::archive_options_read *read_options);
    // list archive content
//    int list();
    // list only children of parent dir
    int list_children(const char *dir);
#ifdef GET_CHILDREN_IN_TABLE
    std::vector<libdar::list_entry> get_children_in_table ( const std::string & dir) const;
#endif
    int extract(const char *dir,const char *dest,libdar::statistics *stats);
    int count_files_in_dir(const char *dir);
    void get_stats();
    bool test();

    void setListingBuffer(std::list<File> *buffer);
private:
//    std::string get_slice(std::string path);
    libdar::statistics *my_statistic;
    // security
    int block_size;
    Glib::ustring pass;
    libdar::crypto_algo crypt_algo;
    Window *parentWindow;
};

#endif

