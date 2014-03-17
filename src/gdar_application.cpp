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
#include "gdar_application.hpp"
#include "libgdar.hpp"

GdarApplication::GdarApplication() {
    Glib::set_application_name("Gdar");
    myTheme = Gtk::IconTheme::get_default();
}

GdarApplication::~GdarApplication() {
}

Glib::RefPtr<GdarApplication> GdarApplication::create() {
    return Glib::RefPtr<GdarApplication>( new GdarApplication() );
}

void GdarApplication::on_startup() {

    Gtk::Application::on_startup();
    add_action("quit", sigc::mem_fun(*this, &GdarApplication::on_action_quit) );
    add_action("info", sigc::mem_fun(*this, &GdarApplication::on_action_info) );

    Glib::RefPtr<Gio::Menu> app_menu = Gio::Menu::create();
    app_menu->append(_("Info"), "app.info");
    app_menu->append(_("Quit"), "app.quit");

    set_app_menu(app_menu);
}

void GdarApplication::create_window() {
    window = new GdarOpenWindow();
    window->set_default_size(400, 550);
    add_window(*window);

    window->signal_hide().connect(sigc::bind<Gtk::Window*>(sigc::mem_fun(*this, &GdarApplication::on_window_hide), window));

    window->show();
}

void GdarApplication::on_window_hide(Gtk::Window* window) {
    delete window;
}

void GdarApplication::on_activate() {
    create_window();
}

void GdarApplication::on_action_quit() {
    quit();
}

void GdarApplication::on_action_info() {
    Gtk::AboutDialog aboutDialog;

    aboutDialog.set_logo(myTheme->load_icon("emblem-package",100));
    aboutDialog.set_program_name("Gdar");
    aboutDialog.set_version(VERSION);
    aboutDialog.set_copyright("Tobias Specht");
    aboutDialog.set_comments(_("A graphical user interface to brows and extract dar archives."));
    aboutDialog.set_license_type(Gtk::LICENSE_GPL_3_0);

    aboutDialog.set_website("https://www.github.com/peckto/gdar");
    aboutDialog.set_website_label(_("Gdar on GitHub"));

    std::vector<Glib::ustring> list_authors;
    list_authors.push_back("Tobias Specht");
    aboutDialog.set_authors(list_authors);

    aboutDialog.set_transient_for(*window);
    aboutDialog.run();
}

