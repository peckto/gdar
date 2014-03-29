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
}
Mydar::Mydar(std::string path, std::string slice) {
    this->path = path;
    this->slice = slice;
}

Mydar::~Mydar() {
    if (my_statistic != NULL) {
        delete my_statistic;
    }
    libdar::close_and_clean();
}

int Mydar::init() {
    dialog = Dialog();
    dialog_custom_listing = Dialog_custom_listing();
    my_statistic = NULL;
    stats_total = "";
    libdar::U_I maj, med, min;
    libdar::U_16 excode;
    std::string msg;

    libdar::get_version_noexcept(maj, med, min, excode, msg);

    if(excode != LIBDAR_NOEXCEPT) {
        std::cout << msg << endl;
        return 1;
    }

    if(maj != LIBDAR_COMPILE_TIME_MAJOR || med < libdar::LIBDAR_COMPILE_TIME_MEDIUM) {
        std::cout << "we are linking against wrong libdar" << std::endl;
        return 1;
    }
    return 0;
}

int Mydar::open() {
    my_arch = libdar::open_archive_noexcept(dialog,path,slice, "dar", archive_options_read(), exception, except_msg); 

    if(exception != LIBDAR_NOEXCEPT) {
        std::cout << "an error occurred: " << except_msg << std::endl;
        return 1;
    }
//    my_arch->init_catalogue(dialog); // not avalible in libdar v5.3.2
    return 0;
}

int Mydar::list() {
    op_listing_noexcept(dialog_custom_listing,my_arch,
        archive_options_listing(),
        exception,
        except_msg);
    if(exception != LIBDAR_NOEXCEPT) {
        std::cout << "an error occurred: " << except_msg << endl;
        return 1;
    }
    return 0;
}

int Mydar::list_children(const char *dir) {
    get_children_of_noexcept(dialog_custom_listing, my_arch, dir, exception, except_msg);
    if(exception != LIBDAR_NOEXCEPT) {
        std::cout << "an error occurred: " << except_msg << endl;
        return 1;
    }
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
    libdar::op_extract_noexcept(dialog,my_arch,dest,options,stats,exception, except_msg);
    
    if (exception != LIBDAR_NOEXCEPT) {
        std::cout << "an error occourred: " << except_msg << endl;
        return 1;
    }

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
    libdar::statistics test_stats = libdar::op_test_noexcept(dialog, my_arch, test_options, NULL, 
        exception, except_msg);
    if (exception != LIBDAR_NOEXCEPT) {
        std::cout << "an error occurred: " << except_msg << std::endl;
        return false;
    }
    return true;
}

#ifdef GET_CHILDREN_IN_TABLE
std::vector<libdar::list_entry> Mydar::get_children_in_table(const std::string &dir) const {
    std::vector<libdar::list_entry> children_table;
    try {
    children_table = my_arch->get_children_in_table(dir);
    } catch (libdar::Egeneric &e) {
        std::cout << "exception get_children_in_table:" <<e.get_message() << std::endl;
    }
    return children_table;
}
#endif

