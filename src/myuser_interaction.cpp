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

#include "myuser_interaction.hpp"

using namespace std;
using namespace libdar;

File::File(const std::string & flag,
         const std::string & perm,
         const std::string & uid,
         const std::string & gid,
         const std::string & size,
         const std::string & date,
         const std::string & filename,
         bool is_dir,
         bool has_children) {
    this->flag = flag;
    this->perm = perm;
    this->uid = uid;
    this->gid = gid;
    this->size = size;
    this->date = date;
    this->filename = filename;
    this->is_dir = is_dir;
    this->has_children = has_children;
}

void Dialog::pause(const std::string & message) {
    cout << "pause: " << message << endl;
}

void Dialog::warning(const std::string & message) {
    cout << "warning: " << message << endl;
}

std::string Dialog::get_string(const std::string & message, bool echo) {
    cout << "get_string: " << message << endl;
    return "test";
}

libdar::secu_string Dialog::get_secu_string(const std::string &message, bool echo) {
    cout << "get_secu_string: " << message << endl;
    return secu_string("test",4);
}

void Dialog::warning_callback(const std::string &x, void *context) {
    cout << "WARNING: " << x << endl;
}

bool Dialog::answer_callback(const std::string &x, void *context) {
    cout << "ANSWER: " << x << endl;
    return false;
}

string Dialog::string_callback(const std::string &x, bool echo, void *context) {
    cout << "string_callback: " << x << endl;
    return string("test");
}

secu_string Dialog::sec_string_callback(const std::string &x, bool echo, void *context) {
    cout << "string_callback: " << x << endl;
    return secu_string("test",5);
}

void Dialog_custom_listing::listing(const std::string & flag,
                        const std::string & perm,
                        const std::string & uid,
                        const std::string & gid,
                        const std::string & size,
                        const std::string & date,
                        const std::string & filename,
                        bool is_dir,
                        bool has_children) {
    if (listingBuffer) {
        listingBuffer->push_back(File(flag,perm,uid,gid,size,date,filename,is_dir,has_children));
    }
}

Dialog::Dialog() {
    listingBuffer = 0;
}

Dialog_custom_listing::Dialog_custom_listing() {
    listingBuffer = 0;
    set_use_listing(true);
}

Dialog::~Dialog() {}
Dialog_custom_listing::~Dialog_custom_listing() {}

Dialog *Dialog::clone() const
{
    Dialog *ret = new Dialog(*this);
    if(ret == NULL)
        throw Ememory("user_interaction_callback::clone");
    else
    return ret;
}

Dialog_custom_listing *Dialog_custom_listing::clone() const
{
    Dialog_custom_listing *ret = new Dialog_custom_listing(*this);
    if(ret == NULL)
        throw Ememory("user_interaction_callback::clone");
    else
    return ret;
}

void Dialog::inherited_warning(const string& message) {
//    cout << "inherited_warning: " << message << endl;
}

void Dialog::setListingBuffer(std::list<File> *buffer) {
    listingBuffer = buffer;
}

