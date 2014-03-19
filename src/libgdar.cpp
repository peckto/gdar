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


#include <iostream>
#include <glibmm/i18n.h>
#include "libgdar.hpp"
#include "mylibdar.hpp"

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
 
GdarOpenWindow::GdarOpenWindow() :
    img_up(Gtk::Stock::GO_UP, Gtk::ICON_SIZE_BUTTON),
    n_separator(Gtk::ORIENTATION_VERTICAL),
    a_separator1(Gtk::ORIENTATION_VERTICAL),
    a_separator2(Gtk::ORIENTATION_VERTICAL),
    a_separator3(Gtk::ORIENTATION_VERTICAL),
    a_extract(_("Extract")),
    a_open(Gtk::Stock::OPEN),
    grey(),white(),
    a_info_img(Gtk::Stock::INFO, Gtk::ICON_SIZE_BUTTON)
{
    newDar = NULL;
    openTreadActive = false;
    extractThreadActive = false;
    is_open = false;
    extract_stats = NULL;
    // Icon
    myTheme = Gtk::IconTheme::get_default();

    //init Window
    set_title("Gdar");
    set_icon(myTheme->load_icon("emblem-package",50));
    m_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
    i_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,0));
    n_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,0));
    a_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,0));
    m_statusbar.push(_("Please choose a Dar file to open"));

    treeView.signal_row_activated().connect(sigc::mem_fun(*this,&GdarOpenWindow::on_active_row));
    n_button_up.signal_clicked().connect(sigc::mem_fun(*this, &GdarOpenWindow::on_button_up));
    a_open.signal_clicked().connect(sigc::mem_fun(*this, &GdarOpenWindow::on_button_open));
    a_extract.signal_clicked().connect(sigc::mem_fun(*this, &GdarOpenWindow::on_extract));
    a_info.signal_clicked().connect(sigc::mem_fun(*this, &GdarOpenWindow::on_info));
    sw_hide.property_active().signal_changed().connect(sigc::mem_fun(*this, &GdarOpenWindow::on_swh_hide));

    list_children_disp.connect(sigc::mem_fun(*this,&GdarOpenWindow::list_children));
    extract_finish_disp.connect(sigc::mem_fun(*this,&GdarOpenWindow::on_extract_finish));

    int column_count;
    Gtk::TreeViewColumn *column;
    Gtk::CellRendererText *coloureCell;
    Gtk::TreeModelColumn< Glib::RefPtr<Gdk::Pixbuf> > *columnIcon;
//    grey.set_rgb(0xffff,0xffff,0x0000);
    grey.set_grey_p(0.7);
    white.set_grey_p(1);
    // icon
    columnIcon = new Gtk::TreeModelColumn< Glib::RefPtr<Gdk::Pixbuf> >;
    // Name
    coloureCell = new Gtk::CellRendererText;
    column = new Gtk::TreeViewColumn();
    column->set_title(_("Name"));
    column->pack_start(cols.file_icon,false); // create a cell for the icon in the Name column
    column->pack_end(*coloureCell); // create the cell for content of the Name column
    column->add_attribute(coloureCell->property_text(), cols.file_name);
    column->add_attribute(coloureCell->property_background_gdk(), cols.file_colour);
    treeView.append_column(*column);
    // Size
    coloureCell = new Gtk::CellRendererText;
    column_count = treeView.append_column(_("Size"), *coloureCell);
    column = treeView.get_column(column_count -1);
    column->add_attribute(coloureCell->property_text(), cols.file_size);
    column->add_attribute(coloureCell->property_background_gdk(), cols.file_colour);
/*    // Type
    coloureCell = new Gtk::CellRendererText;
    column_count = treeView.append_column("Type", *coloureCell);
    column = treeView.get_column(column_count -1);
    column->add_attribute(coloureCell->property_text(), cols.file_type);
    column->add_attribute(coloureCell->property_background_gdk(), cols.file_colour); */
    // Date
    coloureCell = new Gtk::CellRendererText;
    column_count = treeView.append_column(_("Changed"), *coloureCell);
    column = treeView.get_column(column_count -1);
    column->add_attribute(coloureCell->property_text(), cols.file_changed);
    column->add_attribute(coloureCell->property_background_gdk(), cols.file_colour);
    // layout
    Gtk::TreeViewColumn *firstCol = treeView.get_column(0);
//    firstCol->set_sizing(Gtk::TREE_VIEW_COLUMN_GROW_ONLY);
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
    a_open.set_always_show_image(true);
    a_info.set_always_show_image(true);
    a_info.set_image(a_info_img);
    a_box->pack_start(a_open);
    a_box->pack_start(a_separator1);
    a_box->pack_start(a_extract);
    a_box->pack_start(a_separator2);
    a_box->pack_start(a_info);
    a_box->pack_start(a_separator3);
    a_box->pack_start(sw_hide);

    show_all_children();
}

GdarOpenWindow::~GdarOpenWindow() { 
    if ( extract_stats != NULL )
        delete extract_stats;
}

void GdarOpenWindow::openDar() {
    char *c_path, *c_slice;

    if (newDar != NULL) {
        delete newDar;
    }

    c_path = new char [path.length()+1];
    c_slice = new char [slice.length()+1];
    strcpy(c_path,path.c_str());
    strcpy(c_slice,slice.c_str()); 
    newDar = new Mydar(c_path,c_slice);
    newDar->init();
    newDar->setListingBuffer(&listingBuffer);
    newDar->open(); 
    extract_stats = new libdar::statistics(true);
    is_open = true;
    n_entry_path.set_text("");
    treePath.clear();
    delete c_path;
    delete c_slice; 
}

#ifdef GET_CHILDREN_IN_TABLE
void GdarOpenWindow::populate(std::vector<libdar::list_entry> *children_table) {
    Gtk::TreeModel::Row row;
    listStore->clear();
    int size;
    for (std::vector<libdar::list_entry>::iterator it = children_table->begin();it != children_table->end();it++) {
        row = *(listStore->append());
        row[cols.file_name] = it->get_name();
        row[cols.file_is_dir] = it->is_dir();
        row[cols.file_size] = it->get_file_size();
        if (it->is_dir()) {
            row[cols.file_type] = _("Folder");
            row[cols.file_size] = _("Folder"); // should be amount of sub inodes
            row[cols.file_icon] = myTheme->load_icon("folder-symbolic",ICON_SIZE);
        } else {
            row[cols.file_type] = "File";
            istringstream(it->get_file_size()) >> size;
            row[cols.file_size] = get_human_readable(size);
            row[cols.file_icon] = myTheme->load_icon("folder-documents-symbolic",ICON_SIZE);
        }
#ifdef LIBDAR_WITH_DATE_AS_TIME
        row[cols.file_changed] = get_readable_date(it->get_last_change_s());
#else
        row[cols.file_changed] = it->get_last_change();
#endif
        if (it->has_data_present_in_the_archive()) {
            row[cols.file_colour] = white;
        } else {
            row[cols.file_colour] = grey;
        }
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
            row[cols.file_icon] = myTheme->load_icon("folder-symbolic",ICON_SIZE);
            row[cols.file_size] = _("Folder");
        } else {
            row[cols.file_type] = _("File");
            row[cols.file_icon] = myTheme->load_icon("folder-documents-symbolic",ICON_SIZE);
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

void GdarOpenWindow::list_children() {
    string s_treePath = get_treePath();
#ifdef GET_CHILDREN_IN_TABLE
    std::vector<libdar::list_entry> children_table = newDar->get_children_in_table(s_treePath);
    populate(&children_table);
#else
    listingBuffer.clear();
    newDar->list_children(s_treePath.c_str());
    populate();
#endif
    n_entry_path.set_text(s_treePath);
}

void GdarOpenWindow::on_active_row(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) {
    string parent;
    lastScroll = scrollAdjust->get_value();
//    cout << "get_vadjustment: " << lastScroll << endl;
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
    openDar();
    list_children_disp();
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

/* FileChooserDialog -> on_button_open() -> openDarThread() -> openDar() */
void GdarOpenWindow::on_button_open() {
    Gtk::FileChooserDialog dialog(_("Please choose a Dar file to open"), Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

    Glib::RefPtr<Gtk::FileFilter> filter = Gtk::FileFilter::create();
    filter->set_name(_("Dar files"));
    filter->add_pattern("*.dar");
    dialog.add_filter(filter);

    int result = dialog.run();
    if (result == Gtk::RESPONSE_OK) {
        string filename = dialog.get_filename();
        int i = filename.find_last_of("/");
        path = filename.substr(0,i);
        slice = filename.substr(i+1,filename.length());
        i = slice.find_first_of(".");
        slice = slice.substr(0,i);
        set_title(slice);
        // start thread
        openThreadPtr = Glib::Thread::create(sigc::mem_fun(*this,&GdarOpenWindow::openDarThread),true);
    }
}

void GdarOpenWindow::on_extract() {
    if (extractThreadActive) {
        Gtk::MessageDialog dlg(_("Extracting is still running"), false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true );
        dlg.set_title(_("Error"));
        dlg.run();
        return;
    }
    Gtk::FileChooserDialog dialog(_("Please choole a destination folder"), Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    dialog.set_transient_for(*this);

    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(_("OK"), Gtk::RESPONSE_OK);

    if (not is_open) return;
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
    newDar->extract(ext_src.c_str(),ext_dest.c_str(),extract_stats);
    m_statusbar.push(_("Ready"));
    m_spinner.stop();
    extract_finish_disp();
    extractThreadActive = false;
}

void GdarOpenWindow::on_extract_finish() {
    Gtk::Label t1( _("Treated files"),Gtk::ALIGN_END,Gtk::ALIGN_START);
    t1.set_size_request(150,-1);
//    Gtk::Label t2( _("Hard links"),Gtk::ALIGN_END,Gtk::ALIGN_START);
    Gtk::Label t3( _("Skipped files"),Gtk::ALIGN_END,Gtk::ALIGN_START);
    Gtk::Label t4( _("Ignored files"),Gtk::ALIGN_END,Gtk::ALIGN_START);
//    Gtk::Label t5( _("tooold"),Gtk::ALIGN_END,Gtk::ALIGN_START);
    Gtk::Label t6( _("Errored files"),Gtk::ALIGN_END,Gtk::ALIGN_START);
//    Gtk::Label t7( _("ea_treated"),Gtk::ALIGN_END,Gtk::ALIGN_START);
//    Gtk::Label t8( _("Processed bytes"),Gtk::ALIGN_END,Gtk::ALIGN_START);
    Gtk::Label tt("\t");
    Gtk::Label l1(libdar::deci(extract_stats->get_treated()).human(),Gtk::ALIGN_START,Gtk::ALIGN_START);
    l1.set_size_request(40,-1);
//    Gtk::Label l2(libdar::deci(extract_stats->get_hard_links()).human(),Gtk::ALIGN_START,Gtk::ALIGN_START);
    Gtk::Label l3(libdar::deci(extract_stats->get_skipped()).human(),Gtk::ALIGN_START,Gtk::ALIGN_START);
    Gtk::Label l4(libdar::deci(extract_stats->get_ignored()).human(),Gtk::ALIGN_START,Gtk::ALIGN_START);
//    Gtk::Label l5(libdar::deci(extract_stats->get_tooold()).human(),Gtk::ALIGN_START,Gtk::ALIGN_START);
    Gtk::Label l6(libdar::deci(extract_stats->get_errored()).human(),Gtk::ALIGN_START,Gtk::ALIGN_START);
//    Gtk::Label l7(libdar::deci(extract_stats->get_ea_treated()).human(),Gtk::ALIGN_START,Gtk::ALIGN_START);
//    Gtk::Label l8(libdar::deci(extract_stats->get_byte_amount()).human(),Gtk::ALIGN_START,Gtk::ALIGN_START);
    Gtk::Table table(8,3,false);
    Gtk::Box *box;

    string msg = ext_src;
    msg += " => ";
    msg += ext_dest;
    Gtk::MessageDialog dlg(msg, false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true );
    box = dlg.get_content_area();
    box->add(table);
    table.attach(t1,0,1,0,1);
//    table.attach(t2,0,1,1,2);
    table.attach(t3,0,1,2,3);
    table.attach(t4,0,1,3,4);
//    table.attach(t5,0,1,4,5);
    table.attach(t6,0,1,5,6);
//    table.attach(t7,0,1,6,7);
//    table.attach(t8,0,1,8,9);
    table.attach(tt,1,2,0,1);
    table.attach(l1,2,3,0,1);
//    table.attach(l2,2,3,1,2);
    table.attach(l3,2,3,2,3);
    table.attach(l4,2,3,3,4);
//    table.attach(l5,2,3,4,5);
    table.attach(l6,2,3,5,6);
//    table.attach(l7,2,3,6,7);
//    table.attach(l8,2,3,8,9);
    dlg.set_title(_("Extract successfully"));
    box->show_all();
    dlg.run();
}

void GdarOpenWindow::on_info() {
    if ( not is_open )
        return;
//    m_spinner.start();
    newDar->get_stats();
//    cout << newDar->stats_total << endl;
//    m_spinner.stop();
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

