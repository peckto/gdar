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


#ifndef MYLIBDAR_HPP
#define MYLIBDAR_HPP

#include "config.h"
#include "libdar_namespace.hpp"
#include <dar/deci.hpp>
#include <libintl.h>
#include "myuser_interaction.hpp"
#include "enc_dialog.hpp"
#include "window.hpp"

#define DEFAULT_CRYPTO_SIZE 10240


class Mydar {
public:
    LIBDAR::archive *my_arch;
    Dialog dialog;
    LIBDAR::U_16 exception;
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
    int open(std::string path, std::string slice, LIBDAR::archive_options_read *read_options);
    std::vector<LIBDAR::list_entry> get_children_in_table ( const std::string & dir) const;
    int extract(const char *dir,const char *dest,LIBDAR::statistics *stats, bool flat = false);
    int count_files_in_dir(const char *dir);
    void get_stats();
    bool test();

private:
    LIBDAR::statistics *my_statistic;
    // security
    int block_size;
    Glib::ustring pass;
    LIBDAR::crypto_algo crypt_algo;
    Window *parentWindow;
};

#endif

