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


#include <iostream>
#include <dirent.h>
#include "mylibdar.hpp"

using namespace std;
using namespace libdar;

Mydar::Mydar() {
    crypt_algo = libdar::crypto_none;
}
Mydar::Mydar(std::string path, std::string slice) : Mydar() {
    this->path = path;
    this->slice = slice;
}

Mydar::~Mydar() {
    if (my_statistic != NULL) {
        delete my_statistic;
    }
//    libdar::close_and_clean();
}

int Mydar::init() {
    dialog = Dialog();
    dialog_custom_listing = Dialog_custom_listing();
    my_statistic = NULL;
    stats_total = "";
//    libdar::U_I maj, med, min;
    libdar::U_16 excode;
    std::string msg;

/*    libdar::get_version(maj, med, min);

    if(maj != LIBDAR_COMPILE_TIME_MAJOR || med < libdar::LIBDAR_COMPILE_TIME_MEDIUM) {
        throw libdar::Erange("initialization", "we are linking against a wrong libdar"); 
    }*/
    return 0;
}

int Mydar::open(std::string path, std::string slice, libdar::archive_options_read *read_options) {
    this->path = path;
    this->slice = slice;

    my_arch = new libdar::archive(dialog,path,slice, "dar", *read_options); 

//    my_arch->init_catalogue(dialog); // not avalible in libdar v5.3.2
    return 0;
}

/*int Mydar::list() {
    my_arch->op_listing(dialog_custom_listing, archive_options_listing());
    return 0;
}*/

int Mydar::list_children(const char *dir) {
    my_arch->get_children_of(dialog_custom_listing, dir);
    return 0;
}

void Mydar::setListingBuffer(std::list<File> *buffer) {
    dialog_custom_listing.setListingBuffer(buffer);
}

int Mydar::extract(const char *dir, const char *dest,libdar::statistics *stats) {
    string dir2 = dest;
    dir2 +=dir; // libdar expects the full path to where the dir will be extracted
    if (my_statistic != NULL) {
        delete my_statistic;
    }

    libdar::archive_options_extract options;
    options.set_subtree(libdar::simple_path_mask(dir2, true));
    options.set_display_skipped(true);
    my_arch->op_extract(dialog,dest,options,stats);
    
    return 0;
}

int Mydar::count_files_in_dir(const char *dir) {
   return 0;
}

/* load statistics of an already open archive */
void Mydar::get_stats() {
    libdar::entree_stats my_stats = my_arch->get_stats();
//    std::cout << "[+] total: " << libdar::deci(my_stats.total).human() << std::endl;
}

/* 
 * test an already open archive
 */
bool Mydar::test() {
    libdar::archive_options_test test_options;
//    libdar::statistics test_stats;
    if (my_statistic != NULL) {
        delete my_statistic;
    }
    libdar::statistics test_stats = my_arch->op_test(dialog, test_options, NULL);

    return true;
}

#ifdef GET_CHILDREN_IN_TABLE
std::vector<libdar::list_entry> Mydar::get_children_in_table(const std::string &dir) const {
    std::vector<libdar::list_entry> children_table;
    children_table = my_arch->get_children_in_table(dir);
    return children_table;
}
#endif

/*
void Mydar::set_crypto_size(int size) {
    block_size = size;
}

void Mydar::set_crypto_pass(Glib::ustring pass) {
    this->pass = pass;
}

void Mydar::set_crypto_algo(libdar::crypto_algo algo) {
    crypt_algo = algo;
}

*/
