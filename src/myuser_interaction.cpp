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


bool Dialog::inherited_pause(const std::string & message) {
    cout << "pause: " << message << endl;
    return false;
}

void Dialog::inherited_message(const std::string & message) {
    cout << "warning: " << message << endl;
}

std::string Dialog::inherited_get_string(const std::string & message, bool echo) {
    (void)echo;
    cout << "get_string: " << message << endl;
    return "";
}

LIBDAR::secu_string Dialog::inherited_get_secu_string(const std::string &message, bool echo) {
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

Dialog::Dialog(Window *parentWindow) {
    this->parentWindow = parentWindow;
}

Dialog::~Dialog() {}

Dialog *Dialog::clone() const
{
    Dialog *ret = new Dialog(*this);
    if(ret == NULL)
        throw Ememory("user_interaction_callback::clone");
    else
    return ret;
}
