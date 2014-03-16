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


#ifndef MYLIBDAR_HPP
#define MYLIBDAR_HPP

#include <dar/libdar.hpp>
#include <dar/deci.hpp>
#include <libintl.h>
#include "config.h"

class File {
public:
    std::string flag;
    std::string perm;
    std::string uid;
    std::string gid;
    std::string size;
    std::string date;
    std::string filename;
    bool is_dir;
    bool has_children;

    File(const std::string & flag,
         const std::string & perm,
         const std::string & uid,
         const std::string & gid,
         const std::string & size,
         const std::string & date,
         const std::string & filename,
         bool is_dir,
         bool has_children);
};

class Dialog : public libdar::user_interaction {
public:
    std::list<File> *listingBuffer;
    void pause(const std::string & message);
    void warning(const std::string & message);
    std::string get_string(const std::string & message, bool echo);
    libdar::secu_string get_secu_string(const std::string &message, bool echo);
    void warning_callback(const std::string &x, void *context);
    bool answer_callback(const std::string &x, void *context);
    std::string string_callback(const std::string &x, bool echo, void *context);
    libdar::secu_string sec_string_callback(const std::string &x, bool echo, void *context);
    Dialog();
    ~Dialog();
    Dialog *clone() const;
    void setListingBuffer(std::list<File> *buffer);
protected:
    void inherited_warning(const std::string& message);
};

class Dialog_custom_listing : public Dialog {
public:
    void listing(const std::string & flag,
                            const std::string & perm,
                            const std::string & uid,
                            const std::string & gid,
                            const std::string & size,
                            const std::string & date,
                            const std::string & filename,
                            bool is_dir,
                            bool has_children);
    Dialog_custom_listing();
    ~Dialog_custom_listing();
    Dialog_custom_listing *clone() const;
};

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

    Mydar();
    Mydar(std::string path, std::string slice);
    ~Mydar();
    // init libdar
    int init();
    // open dar archive
    int open();
    // list archive content
    int list();
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
};

#endif
