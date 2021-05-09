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


#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "config.h"
#include <glibmm/i18n.h>
#include <gtkmm.h>
#include <string>
#include "pwd_dialog.hpp"


class Window : public Gtk::ApplicationWindow {
public:
    Glib::RefPtr<Gtk::ListStore> listStore;

    Glib::Dispatcher show_pwd_dialog_disp;
    Glib::Threads::Mutex dialog_mutex;
    Glib::ustring dialog_secu_string;

    Window();
    ~Window();

    std::string path,slice;

protected:
    //Signal handlers
    void on_show_pwd_dialog();

    //Member widgets
    Gtk::Box *m_box; //main Box
    // Action widgets
    Gtk::Box *a_box;
    Gtk::Button a_open;
    Gtk::Button a_extract;
    Gtk::Separator a_separator1;
    Gtk::Separator a_separator2;
    Gtk::Separator a_separator3;
    Gtk::Button a_info;
    Gtk::Image a_info_img;
    Gtk::CheckButton cb_hide;
    // Main widgets
    Gtk::VBox topBox;
    Gtk::Box *n_box; //navigation Box
    Gtk::ScrolledWindow scrollWindow;
    Glib::RefPtr<Gtk::Adjustment>  scrollAdjust;
    double lastScroll;
    Gtk::TreeView treeView;
    Glib::RefPtr<Gtk::TreeModelSort> filterSort;
    Glib::RefPtr<Gtk::TreeModelFilter> filter;
    // navigation widgets
    Gtk::Button n_button_up;
    Gtk::Image img_up;
    Gtk::Entry n_entry_path;
    // Info widgets
    Gtk::Box *i_box; //info Box
    Gtk::Statusbar m_statusbar;
    Gtk::Spinner m_spinner;
    Gtk::Separator n_separator;
    // Threading
    Glib::Dispatcher list_children_disp;
    Glib::Dispatcher extract_finish_disp;
    Glib::Thread *openThreadPtr;
    // color
    Gdk::Color grey,white;

    std::string ext_src, ext_dest;
    std::list<std::string> treePath;

    Glib::Mutex errorMutex;
    Glib::Dispatcher show_error_dialog_disp;

}; 

#endif

