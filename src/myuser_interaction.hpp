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

#ifndef MYUSER_INTERACTION_HPP
#define MYUSER_INTERACTION_HPP

#include "config.h"
#include "window.hpp"
#include "file.hpp"
#include "libdar_namespace.hpp"
#include <iostream>


class Dialog : public LIBDAR::user_interaction {
protected:
        // display of informational message
    virtual void inherited_message(const std::string & message) override;

        // display of a question and returns the answer from user as true/false
    virtual bool inherited_pause(const std::string & message) override;

        // display the message and returns a string from the user,
        // with or without display what the user typed (echo)
    virtual std::string inherited_get_string(const std::string & message, bool echo) override;

        // same as the previous be the user provided string is returned as secu_string
    virtual libdar::secu_string inherited_get_secu_string(const std::string & message, bool echo) override;

public:
    Dialog(Window *parentWindow);
    ~Dialog();
    Dialog *clone() const;
protected:
    Window *parentWindow;
};

#endif // MYUSER_INTERACTION_HPP

