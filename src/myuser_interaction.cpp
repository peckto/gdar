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

#include <glibmm/threads.h>
#include "myuser_interaction.hpp"

using namespace std;
using namespace LIBDAR;


void Dialog::pause(const std::string & message) {
    cout << "pause: " << message << endl;
}

void Dialog::warning(const std::string & message) {
    cout << "warning: " << message << endl;
}

std::string Dialog::get_string(const std::string & message, bool echo) {
    (void)echo;
    cout << "get_string: " << message << endl;
    return "";
}

LIBDAR::secu_string Dialog::get_secu_string(const std::string &message, bool echo) {
    (void)message;
    (void)echo;
    //cout << "get_secu_string: " << message << endl;
    parentWindow->dialog_mutex.lock();
    parentWindow->show_pwd_dialog_disp();
    {
    Glib::Threads::Mutex::Lock lock(parentWindow->dialog_mutex);
    }
    LIBDAR::secu_string s(parentWindow->dialog_secu_string.c_str(),parentWindow->dialog_secu_string.length());
    return s;
}

void Dialog::warning_callback(const std::string &x, void *context) {
    (void)context;
    cout << "WARNING: " << x << endl;
}

bool Dialog::answer_callback(const std::string &x, void *context) {
    (void)context;
    cout << "ANSWER: " << x << endl;
    return false;
}

string Dialog::string_callback(const std::string &x, bool echo, void *context) {
    (void)echo;
    (void)context;
    cout << "string_callback: " << x << endl;
    return string("");
}

secu_string Dialog::sec_string_callback(const std::string &x, bool echo, void *context) {
    (void)echo;
    (void)context;
    cout << "string_callback: " << x << endl;
    return secu_string("",0);
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

Dialog::Dialog(Window *parentWindow) {
    listingBuffer = 0;
    this->parentWindow = parentWindow;
}

Dialog_custom_listing::Dialog_custom_listing(Window *parentWindow) : Dialog(parentWindow) {
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
    (void)message;
//    cout << "inherited_warning: " << message << endl;
}

void Dialog::setListingBuffer(std::list<File> *buffer) {
    listingBuffer = buffer;
}

