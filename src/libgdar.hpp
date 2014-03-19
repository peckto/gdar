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


#ifndef LIBGDAR_HPP
#define LIBGDAR_HPP

#include <gtkmm.h>
#include <math.h> // log10
#include <string>
#include "mylibdar.hpp"
#include "gdar_application.hpp"
#include "config.h"
#define ICON_SIZE 18

class FileColumns : public Gtk::TreeModel::ColumnRecord {
public:
    FileColumns();
    ~FileColumns();

    Gtk::TreeModelColumn< Glib::RefPtr<Gdk::Pixbuf> > file_icon;
    Gtk::TreeModelColumn<std::string> file_name;
    Gtk::TreeModelColumn<std::string> file_size;
    Gtk::TreeModelColumn<std::string> file_type;
    Gtk::TreeModelColumn<std::string> file_changed;
    Gtk::TreeModelColumn<Gdk::Color> file_colour;
    Gtk::TreeModelColumn<bool> file_is_dir;
};

class GdarOpenWindow : public Gtk::ApplicationWindow {
public:
    Glib::RefPtr<Gtk::ListStore> listStore;
    FileColumns cols;

    std::list<File> listingBuffer;
    Mydar *newDar;
 
    GdarOpenWindow(GdarApplication *application);
    ~GdarOpenWindow();

    void openDar();
    void openDarThread();
    void populate();
#ifdef GET_CHILDREN_IN_TABLE
    void populate(std::vector<libdar::list_entry> *children_table);
#endif
    void on_active_row(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
    void list_children();
protected:
    //Signal handlers
    void on_exit_click();
    void on_button_up();
    void on_button_open();
    void on_extract();
    void on_info();
    void on_swh_hide();
    bool filter_func(Gtk::TreeModel::const_iterator it);

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
    Gtk::Switch sw_hide;
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
private:
    libdar::statistics *extract_stats;
    std::string get_treePath();
    bool openTreadActive;
    bool extractThreadActive;
    void extractThread();
    void on_extract_finish();
    bool is_open;
    std::string path,slice;
    std::string ext_src, ext_dest;
    std::list<std::string> treePath;
    int sort_func(const Gtk::TreeModel::iterator &col1, const Gtk::TreeModel::iterator col2);
    std::string get_human_readable(int size);
    std::string get_readable_date(time_t date);
    void on_about_dialog();

    GdarApplication *gdarApp;
}; 

#endif

