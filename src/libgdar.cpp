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


#include <iostream>
#include <iomanip>
#include "libgdar.hpp"
#include "mylibdar.hpp"
#include "enc_dialog.hpp"

using namespace std;

FileColumns::FileColumns() {
    add(file_name);
    add(file_size);
    add(file_type);
    add(file_changed);
    add(file_is_dir);
    add(file_colour);
    add(file_icon);
}
FileColumns::~FileColumns() {
}
 
GdarOpenWindow::GdarOpenWindow(GdarApplication *application) : Window()
{
    newDar = NULL;
    openTreadActive = false;
    extractThreadActive = false;
    is_open = false;
    extract_stats = NULL;
    gdarApp = application;
    gdarApp->myTheme = Gtk::IconTheme::get_default(); // In older gtkmm versions it seems like 
                                                      // the IconTheme can only be fetched inside the Window

    //init Window
    set_title("Gdar");
    set_icon(gdarApp->myTheme->load_icon("emblem-package",50));
    m_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
    i_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,0));
    n_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,0));
    a_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,0));
    m_statusbar.push(_("Please choose a Dar file to open"));

    a_open.set_tooltip_text(_("Open archive"));
    sw_hide.set_tooltip_text(_("Show hidden files"));
    a_info.set_tooltip_text(_("Show information about archive"));
    a_extract.set_tooltip_text(_("Extract selected file or folder"));
    n_button_up.set_tooltip_text(_("Change to parent directory"));

    treeView.signal_row_activated().connect(sigc::mem_fun(*this,&GdarOpenWindow::on_active_row));
    n_button_up.signal_clicked().connect(sigc::mem_fun(*this, &GdarOpenWindow::on_button_up));
    a_open.signal_clicked().connect(sigc::mem_fun(*this, &GdarOpenWindow::on_button_open));
    a_extract.signal_clicked().connect(sigc::mem_fun(*this, &GdarOpenWindow::on_extract));
    a_info.signal_clicked().connect(sigc::mem_fun(*this, &GdarOpenWindow::on_info));
    sw_hide.property_active().signal_changed().connect(sigc::mem_fun(*this, &GdarOpenWindow::on_swh_hide));
    n_entry_path.signal_activate().connect(sigc::mem_fun(*this, &GdarOpenWindow::on_entry_path_activate));

    list_children_disp.connect(sigc::mem_fun(*this,&GdarOpenWindow::list_children_v));
    extract_finish_disp.connect(sigc::mem_fun(*this,&GdarOpenWindow::on_extract_finish));
    show_pwd_dialog_disp.connect(sigc::mem_fun(*this,&GdarOpenWindow::on_show_pwd_dialog));
    show_error_dialog_disp.connect(sigc::mem_fun(*this,&GdarOpenWindow::show_error_dialog));

    int column_count;
    Gtk::TreeViewColumn *column;
    Gtk::CellRendererText *coloureCell;
    grey.set_grey_p(0.7);
    white.set_grey_p(1);
    // Name
    coloureCell = new Gtk::CellRendererText;
    Gtk::CellRendererPixbuf iconCell;
    column = new Gtk::TreeViewColumn();
    column->set_title(_("Name"));
    column->pack_start(iconCell,false);
    column->add_attribute(iconCell.property_pixbuf(), cols.file_icon);
    column->add_attribute(iconCell.property_cell_background_gdk(), cols.file_colour);
    column->pack_end(*coloureCell); // create the cell for content of the Name column
    column->add_attribute(coloureCell->property_text(), cols.file_name);
    treeView.append_column(*column);
    // Size
    coloureCell = new Gtk::CellRendererText;
    column_count = treeView.append_column(_("Size"), *coloureCell);
    column = treeView.get_column(column_count -1);
    column->add_attribute(coloureCell->property_text(), cols.file_size);
    // Date
    coloureCell = new Gtk::CellRendererText;
    column_count = treeView.append_column(_("Changed"), *coloureCell);
    column = treeView.get_column(column_count -1);
    column->add_attribute(coloureCell->property_text(), cols.file_changed);
    // layout
    Gtk::TreeViewColumn *firstCol = treeView.get_column(0);
    firstCol->set_min_width(200);
    firstCol->set_max_width(400);
    firstCol->set_expand(true);
    firstCol->set_sort_column(cols.file_name);

    listStore = Gtk::ListStore::create(cols);
    filter = Gtk::TreeModelFilter::create(listStore);
    filter->set_visible_func(sigc::mem_fun(*this,&GdarOpenWindow::filter_func));
    filterSort = Gtk::TreeModelSort::create(filter);
    filterSort->set_sort_func(0,sigc::mem_fun(*this,&GdarOpenWindow::sort_func));
    filterSort->set_sort_column(0,Gtk::SORT_ASCENDING); // activate sort on the first column
    treeView.set_model(filterSort);

    scrollWindow.add(treeView);
    scrollWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    scrollAdjust = scrollWindow.get_vadjustment();
   
    topBox.pack_start(scrollWindow);
    add(*m_box);
    m_box->pack_start(*a_box,Gtk::PACK_SHRINK);
    m_box->pack_start(*n_box,Gtk::PACK_SHRINK);
    m_box->pack_start(topBox);
    m_box->pack_start(*i_box,Gtk::PACK_SHRINK);

    i_box->pack_start(m_spinner,Gtk::PACK_SHRINK);
    i_box->pack_start(m_statusbar);

    n_button_up.set_image(img_up);
    n_box->pack_start(n_button_up,Gtk::PACK_SHRINK);
    n_box->pack_start(n_separator,Gtk::PACK_SHRINK);
    n_separator.set_margin_left(2);
    n_separator.set_margin_right(2);
    n_box->pack_start(n_entry_path);

    // Action widgets
#ifdef SET_ALWAYS_SHOW_IMAGE
    a_open.set_always_show_image(true);
    a_info.set_always_show_image(true);
#endif
    a_info.set_image(a_info_img);
    a_box->pack_start(a_open);
    a_box->pack_start(a_separator1);
    a_box->pack_start(a_extract);
    a_box->pack_start(a_separator2);
    a_box->pack_start(a_info);
    a_box->pack_start(a_separator3);
    a_box->pack_start(sw_hide);

    show_all_children();

    // init libdar
    LIBDAR::U_I maj, med, min;
    LIBDAR::get_version(maj, med, min);

    if(maj != LIBDAR::LIBDAR_COMPILE_TIME_MAJOR || med < LIBDAR::LIBDAR_COMPILE_TIME_MEDIUM) {
        std::cout << "we are linking against a wrong libdar" << std::endl; // TODO dialog
        Gtk::Main::quit();
    }
    read_options = NULL;
}

GdarOpenWindow::~GdarOpenWindow() { 
    LIBDAR::close_and_clean();

    if ( extract_stats != NULL )
        delete extract_stats;
}

bool GdarOpenWindow::openDar() {
    try {
        newDar->init();
        newDar->setListingBuffer(&listingBuffer);
        newDar->open(path,slice,read_options); 
    } catch (LIBDAR::Egeneric &e) {
        {
            Glib::Mutex::Lock lock(errorMutex);
            ErrorMsg emsg(e);
            errorPipe.push(emsg);
            show_error_dialog_disp();
        }
        return false;
    }
    extract_stats = new LIBDAR::statistics(true);
    is_open = true;
    return true;
}

#ifdef GET_CHILDREN_IN_TABLE
void GdarOpenWindow::populate(std::vector<LIBDAR::list_entry> *children_table) {
    Gtk::TreeModel::Row row;
    listStore->clear();
    int size;
    for (std::vector<LIBDAR::list_entry>::iterator it = children_table->begin();it != children_table->end();it++) {
        row = *(listStore->append());
        row[cols.file_name] = it->get_name();
        row[cols.file_is_dir] = it->is_dir();
        row[cols.file_size] = it->get_file_size();
        if (it->is_dir()) {
            row[cols.file_type] = _("Folder");
            row[cols.file_size] = _("Folder"); // should be amount of sub inodes
            row[cols.file_icon] = gdarApp->myTheme->load_icon("folder-symbolic",ICON_SIZE);
        } else {
            row[cols.file_type] = "File";
            istringstream(it->get_file_size()) >> size;
            row[cols.file_size] = get_human_readable(size);
            row[cols.file_icon] = gdarApp->myTheme->load_icon("folder-documents-symbolic",ICON_SIZE);
        }
#ifdef LIBDAR_WITH_DATE_AS_TIME
        row[cols.file_changed] = get_readable_date(it->get_last_change_s());
#else
        row[cols.file_changed] = it->get_last_change();
#endif
    }
    treeView.columns_autosize();
}
#endif

/*
 * converts a size given in bytes to a human readable string like 10.4 MB
 */
std::string GdarOpenWindow::get_human_readable(int size) {
    if (size == 0) {
        return "0 bytes";
    }
    std::string readable, t;
    char c_redable[6];
    int len, exp;
    float f_readable;
    len = log10(size)+1;
    if (len <= 3) {
        stringstream ss;
        ss << size << " bytes";
        readable = ss.str();
        return readable;
    }
    exp = (len-1)/3;
    f_readable = size/pow(10,exp*3);
    sprintf(c_redable,"%.1f",f_readable);
    switch (exp) {
       case 1: t = "kB"; break;
       case 2: t = "MB"; break;
       case 3: t = "GB"; break;
       case 4: t = "TB"; break;
       default: t = "XX"; break;
    }
    readable = c_redable;
    readable += " ";
    readable += t;
    return readable;
}

/*
 * convert from t_time to easy readable format
 */
std::string GdarOpenWindow::get_readable_date(time_t date) {
    struct tm tm_d;
    struct tm tm_n;
    time_t now;
    char buff[80] = {'\0'};
    std::string readable;

    time(&now);
    tm_n = *localtime(&now);
    tm_d = *localtime(&date);
    if (tm_d.tm_year == tm_n.tm_year) {
        if (tm_d.tm_mday == tm_n.tm_mday) {
            strftime(buff, 20, "%R", &tm_d);
            return buff;
        }
        strftime(buff, 20, "%d. %h", &tm_d);
        return buff;
    }
    strftime(buff, 20, "%d. %h %G", &tm_d);
    return buff;
}

void GdarOpenWindow::populate() {
    Gtk::TreeModel::Row row;
    listStore->clear();
    int size;
    for (std::list<File>::iterator it = listingBuffer.begin();it!=listingBuffer.end();it++) {
        row = *(listStore->append());
        row[cols.file_name] = it->filename;
        row[cols.file_is_dir] = it->is_dir;
        istringstream(it->size) >> size;
        if (it->is_dir) { 
            row[cols.file_type] = _("Folder");
            row[cols.file_icon] = gdarApp->myTheme->load_icon("folder-symbolic",ICON_SIZE);
            row[cols.file_size] = _("Folder");
        } else {
            row[cols.file_type] = _("File");
            row[cols.file_icon] = gdarApp->myTheme->load_icon("folder-documents-symbolic",ICON_SIZE);
            row[cols.file_size] = get_human_readable(size);
        }
        row[cols.file_changed] = it->date;
        row[cols.file_colour] = white;

    }
    treeView.columns_autosize();
}

string GdarOpenWindow::get_treePath() {
    string s_treePath = "";
    for (list<string>::iterator it=treePath.begin();it !=treePath.end();it++) {
        s_treePath += *it+"/";
    }
    return s_treePath;
}

void GdarOpenWindow::list_children_v() {
    list_children();
}

int GdarOpenWindow::list_children() {
    string s_treePath = get_treePath();
    try {
#ifdef GET_CHILDREN_IN_TABLE
        std::vector<LIBDAR::list_entry> children_table = newDar->get_children_in_table(s_treePath);
        populate(&children_table);
#else
        listingBuffer.clear();
        newDar->list_children(s_treePath.c_str());
        populate();
#endif
    } catch (LIBDAR::Egeneric &e) {
        {
            Glib::Mutex::Lock lock(errorMutex);
            ErrorMsg emsg(e);
            errorPipe.push(emsg);
            show_error_dialog_disp();
        }
        return -1;
    }
    n_entry_path.set_text(s_treePath);
    n_entry_path.set_position(s_treePath.size());
    return 0;
}

void GdarOpenWindow::on_active_row(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) {
    (void)path;
    (void)column;
    string parent;
    lastScroll = scrollAdjust->get_value();
    Glib::RefPtr<Gtk::TreeView::Selection> selection = treeView.get_selection();
    Gtk::TreeModel::iterator itr = selection->get_selected();
    Gtk::TreeModel::Row activeRow = *itr;
    if (activeRow[cols.file_is_dir]) {
        parent = activeRow[cols.file_name];
        treePath.push_back(parent);
        list_children();
    }
}

void GdarOpenWindow::openDarThread() {
    m_statusbar.push(_("Reading catalogue"));
    m_spinner.start();
    if ( openDar() ) {
        list_children_disp();
        set_title(slice);
    }
    m_statusbar.push(_("Ready"));
    m_spinner.stop();
}

void GdarOpenWindow::on_button_up() {
    if (treePath.empty()) {
        return;
    }
    treePath.pop_back();
    list_children();
    scrollAdjust->set_value(lastScroll);
}

int GdarOpenWindow::sort_func(const Gtk::TreeModel::iterator &itr1, const Gtk::TreeModel::iterator itr2) {
    const Gtk::TreeModel::Row row1 = *itr1;
    const Gtk::TreeModel::Row row2 = *itr2;
    string name1 = row1[cols.file_name];
    string name2 = row2[cols.file_name];
    bool is_dir1 = row1[cols.file_is_dir];
    bool is_dir2 = row2[cols.file_is_dir];
   
    if ((is_dir1 && is_dir2)||(not is_dir1&&not is_dir2)) {
        int ret = name1.compare(name2);
        if (ret == 0)
           return 0;
        else if (ret < 0)
            return -1;
        else
             return 1;
    } else if (is_dir1)
        return -1;
    else
        return 1;
}

/* FileChooserDialog -> on_button_open() -> open() -> openDarThread() -> openDar() */
void GdarOpenWindow::on_button_open() {
    Gtk::FileChooserDialog dialog(_("Please choose a Dar file to open"), Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
    Gtk::CheckButton enc_check;
    Gtk::ButtonBox *action_aria;
    enc_check.set_label(_("Backup is encrypted"));
    enc_check.show();
    action_aria = dialog.get_action_area();
    action_aria->pack_end(enc_check);

    Glib::RefPtr<Gtk::FileFilter> filter = Gtk::FileFilter::create();
    filter->set_name(_("Dar files"));
    filter->add_pattern("*.dar");
    dialog.add_filter(filter);

    int result = dialog.run();
    dialog.hide();
    if (result == Gtk::RESPONSE_OK) {
        string filename = dialog.get_filename();
        if ( enc_check.get_active() ) {
            EncSettings encSettins(*this);
            result = encSettins.run();
            if (result != Gtk::RESPONSE_OK) {
                return;
            }
            open(filename, &encSettins);
        } else
            open(filename, NULL);
    }
}

void GdarOpenWindow::open(string &filename, EncSettings *encSettins) {
    create_mydar();
    LIBDAR::secu_string tmp_pass;
    LIBDAR::U_32 crypto_size = DEFAULT_CRYPTO_SIZE;

    if (encSettins != NULL ) {
        read_options->set_crypto_size(encSettins->get_block_size());
        read_options->set_crypto_pass(encSettins->get_pass());
        read_options->set_crypto_algo(encSettins->get_crypt_algo());
    }

    int i = filename.find_last_of("/");
    path = filename.substr(0,i);
    slice = filename.substr(i+1,filename.length());
    // archive name like: <name>.<slice>.dar (home-2014-09-14.1.dar)
    // be aware of dots occurring inside name!
    i = slice.find_last_of(".");
    i = slice.find_last_of(".", i-1);
    slice = slice.substr(0,i);
    // start thread
    openThreadPtr = Glib::Thread::create(sigc::mem_fun(*this,&GdarOpenWindow::openDarThread),true);
}

void GdarOpenWindow::create_mydar() {
    if (newDar != NULL) {
        delete newDar;
    }
    if (read_options != NULL) {
        delete read_options;
    }
    read_options = new LIBDAR::archive_options_read;
    newDar = new Mydar(this);
    n_entry_path.set_text("");
    treePath.clear();
    listStore->clear();
    is_open = false;
    set_title("Gdar");
}

void GdarOpenWindow::on_extract() {
    if (extractThreadActive) {
        Gtk::MessageDialog dlg(_("Extracting is still running"), false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true );
        dlg.set_title(_("Error"));
        dlg.run();
        return;
    }
    if (not is_open) return;
    Gtk::FileChooserDialog dialog(_("Please choole a destination folder"), Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    dialog.set_transient_for(*this);

    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(_("OK"), Gtk::RESPONSE_OK);

    string s_treePath = get_treePath();
    string file_name;
    Glib::RefPtr<Gtk::TreeView::Selection> selection = treeView.get_selection();
    Gtk::TreeModel::iterator itr = selection->get_selected();
    if (not itr) return;
    Gtk::TreeModel::Row activeRow = *itr;
    file_name = activeRow[cols.file_name];
    ext_src = s_treePath+file_name;

    int res = dialog.run();
    if (res != Gtk::RESPONSE_OK) return;
    ext_dest = dialog.get_filename();
    ext_dest +="/";
    openThreadPtr = Glib::Thread::create(sigc::mem_fun(*this,&GdarOpenWindow::extractThread),true
    );
}

void GdarOpenWindow::extractThread() {
    extractThreadActive = true;
    m_statusbar.push(_("Extracting files"));
    m_spinner.start();
    try {
        newDar->extract(ext_src.c_str(),ext_dest.c_str(),extract_stats);
    } catch (LIBDAR::Egeneric &e) {
        {
            Glib::Mutex::Lock lock(errorMutex);
            ErrorMsg emsg(e);
            errorPipe.push(emsg);
            show_error_dialog_disp();
        }
        return;
    }
    m_statusbar.push(_("Ready"));
    m_spinner.stop();
    extract_finish_disp();
    extractThreadActive = false;
}

void GdarOpenWindow::on_extract_finish() {
    std::map<std::string, std::string> stats;

    stats[_("Treated files")] = LIBDAR::deci(extract_stats->get_treated()).human();
    stats[_("Skipped files")] = LIBDAR::deci(extract_stats->get_skipped()).human();
    stats[_("Ignored files")] = LIBDAR::deci(extract_stats->get_ignored()).human();
    stats[_("Errored files")] = LIBDAR::deci(extract_stats->get_errored()).human();

    string msg = ext_src;
    msg += " => ";
    msg += ext_dest;
    TableDialog dlg(msg,stats);
    dlg.set_title(_("Extract successfully"));
    dlg.run();
}

void GdarOpenWindow::on_info() {
    if ( not is_open )
        return;
    std::map<std::string, std::string> stats;
    LIBDAR::entree_stats my_stats = newDar->my_arch->get_stats();
    stats[_("Total numer of inodes")] = LIBDAR::deci(my_stats.total).human();
    stats[_("Number of directories")] = LIBDAR::deci(my_stats.num_d).human();
    stats[_("Number of files")] = LIBDAR::deci(my_stats.num_f).human();
    stats[_("Saved inodes in this backup")] = LIBDAR::deci(my_stats.saved).human();

    TableDialog dlg("",stats);
    dlg.set_title(_("About ") + slice);
    dlg.run();
}

void GdarOpenWindow::on_swh_hide() {
    filter->refilter();
    scrollAdjust->set_value(0.0);
}

bool GdarOpenWindow::filter_func(Gtk::TreeModel::const_iterator it) {
    if (sw_hide.get_active())
        return true;
    std::string value;
    it->get_value(0,value);
    if (value[0] == '.')
        return false;
    return true;
}

TableDialog::TableDialog(Glib::ustring msg, std::map<std::string, std::string> &cont) :
    Gtk::MessageDialog(msg, false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true ),
    table(cont.size(),3,false),
    tt("\t")
{
    box = get_content_area();
    box->add(table);
    std::map<std::string, std::string>::iterator it;
    Gtk::Label *first, *second;
    int i = 0;
    for (it=cont.begin(); it!=cont.end(); it++) {
        first = new Gtk::Label(it->first,Gtk::ALIGN_END,Gtk::ALIGN_START);
        second = new Gtk::Label(it->second,Gtk::ALIGN_START,Gtk::ALIGN_START);
        labels[first] = second;
        table.attach(*first,0,1,i,i+1);
        table.attach(*second,2,3,i,i+1);
        i++;
    }
    table.attach(tt,1,2,0,1);
    box->show_all();
}

TableDialog::~TableDialog() {
    labelMap::iterator it;
    for (it=labels.begin();it!=labels.end();it++) {
        if (it->first != NULL)
            delete it->first;
        if (it->second != NULL)
            delete it->second;
    }
}

void GdarOpenWindow::show_error_dialog() {
    Glib::ustring msg, source;
    {
        Glib::Mutex::Lock lock(errorMutex);
        ErrorMsg e = errorPipe.front();
        msg = e.msg;
        source = e.source;
        errorPipe.pop();
    }
    Gtk::MessageDialog dlg(*this, msg,false,Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
    dlg.set_title(source);
    dlg.run();
}

int GdarOpenWindow::remove_tail_slash(std::string *path) {
    int i = 0;
    while ((*path)[path->size()] == '/') {
        i++;
        *path = path->substr(0,path->size()-2);
    }
    return i;
}

int GdarOpenWindow::remove_head_slash(std::string *path) {
    int i = 0;
    while ((*path)[0] == '/') {
        i++;
        *path = path->substr(1,path->size()-1);
    }
    return i;
}

std::string GdarOpenWindow::remove_rouble_slash(const std::string &path) {
    char buf[path.size()+1];
    int i=0;
    for (string::const_iterator it=path.begin();it!=path.end();it++) {
        if (i>0) {
            if (buf[i-1] == '/' and *it == '/') {
                continue;
            }
        }
        buf[i] = *it;
        i++;
    }
    buf[i] = '\0';
    return string(buf);
}

void GdarOpenWindow::on_entry_path_activate() {
    if ( not is_open )
        return;
    string path = n_entry_path.get_text();
    list<string> old_path = treePath;
    size_t end;
    size_t start = 0;
    treePath.clear();
    string tmp;
    path = remove_rouble_slash(path);
    remove_tail_slash(&path);
    remove_head_slash(&path);
    end = path.find("/");
    while (end != string::npos) {
        tmp = path.substr(start, end-start);
        end++; // skip '/' at the and of path
        start = end;
        treePath.push_back(tmp);
        end = path.find("/", end);
    }
    if (start != path.size()) {
        treePath.push_back(path.substr(start));
    }
    if (list_children() == -1) {
        treePath = old_path;
        list_children();
    }
}

ErrorMsg::ErrorMsg(Glib::ustring msg, Glib::ustring source) {
    this->msg = msg;
    this->source = source;
}
ErrorMsg::ErrorMsg(LIBDAR::Egeneric &e) {
    msg = e.get_message();
    source = e.get_source();
}

