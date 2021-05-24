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


#ifndef LIBGDAR_HPP
#define LIBGDAR_HPP

#include "config.h"
#include <gtkmm.h>
#include <math.h> // log10
#include <string>
#include <queue>
#include "mylibdar.hpp"
#include "pwd_dialog.hpp"
#include "gdar_application.hpp"
#include "file.hpp"
#include "mylibdar.hpp"
#include "window.hpp"

#define ICON_SIZE 18

class ErrorMsg {
public:
    ErrorMsg(Glib::ustring msg, Glib::ustring source);
    ErrorMsg(LIBDAR::Egeneric &e);
    Glib::ustring msg;
    Glib::ustring source;
};

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

class GdarOpenWindow : public Window {
public:
    FileColumns cols;

    std::list<File> listingBuffer;
    Mydar *newDar;
 
    GdarOpenWindow(GdarApplication *application);
    ~GdarOpenWindow();

    bool parse_dar_file_name(std::string &filename);
    void open(std::string &filename, EncSettings *encSettins);
    bool openDar();
    void openDarThread();
    void populate();
    void populate(std::vector<LIBDAR::list_entry> *children_table);
    void on_active_row(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
    void list_children_v();
    int list_children();
    void create_mydar();

    GdarApplication *gdarApp;

protected:
    //Signal handlers
    void on_exit_click();
    void on_button_up();
    void on_button_open();
    void on_extract();
    void on_info();
    void on_swh_hide();
    bool filter_func(Gtk::TreeModel::const_iterator it);
    void on_entry_path_activate();
    void on_extract_finish();

    std::string get_treePath();
    bool openTreadActive;
    bool extractThreadActive;
    void extractThread(bool flat = false);
    int sort_func(const Gtk::TreeModel::iterator &col1, const Gtk::TreeModel::iterator col2);
    std::string get_human_readable(int size);
    std::string get_readable_date(time_t date);
    void on_about_dialog();
    int remove_tail_slash(std::string *path);
    int remove_head_slash(std::string *path);
    std::string remove_rouble_slash(const std::string &path);

    void show_error_dialog();
    Glib::Mutex errorMutex;
    Glib::Dispatcher show_error_dialog_disp;

private:
    LIBDAR::statistics *extract_stats;
    bool is_open;

    std::queue<ErrorMsg> errorPipe;

    LIBDAR::archive_options_read *read_options;
}; 

class TableDialog : public Gtk::MessageDialog {
public:
    TableDialog(Gtk::Window& parent, Glib::ustring msg, std::map<std::string, std::string> &cont);
    virtual ~TableDialog();
protected:
    typedef std::map<Gtk::Label*, Gtk::Label*> labelMap;
    Gtk::Box *box;
    Gtk::Table table;
    labelMap labels;
    Gtk::Label tt;
};

#endif

