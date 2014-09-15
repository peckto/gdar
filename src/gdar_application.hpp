/*
    gdar - a graphical user interface to browse and extract dar archives
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

#ifndef GDAR_APPLICATION_HPP
#define GDAR_APPLICATION_HPP

#include <gtkmm.h>

class GdarApplication: public Gtk::Application {
protected:
    GdarApplication(int argc, char *argv[]);
public:
    static Glib::RefPtr<GdarApplication> create(int argc, char *argv[]);
    Glib::RefPtr<Gtk::IconTheme> myTheme;

protected:
    ~GdarApplication();
// Overrides of default signal handlers:
    virtual void on_activate();
    virtual void on_startup();
    virtual void on_open(const Gio::Application::type_vec_files &files, const Glib::ustring& hint);

    void on_about_dialog_response(int response_id);

private:
    void create_window();
    void create_window(char *path);
    void on_window_hide(Gtk::Window* window);
    void on_action_quit();
    void on_action_info();

    Gtk::ApplicationWindow *window;
};

#endif

