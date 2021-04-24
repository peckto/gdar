Gdar 
====

![GitHub Workflow Status](https://img.shields.io/github/workflow/status/peckto/gdar/cpp)

A graphical user interface for Linux to browse and extract dar archives

The GUI is written in gtkmm, as backend libdar is used. 
You can use Gdar not only on Gnome desktop but also on KDE or any other Linux desktop environment.
You can find more information at: 
http://www.peckto.de/gdar

Prepare when cloning from git
-----------------------------
```
$ ./autogen.sh
```

Build
-----
```
$ ./configure [--enable-libdar64]
$ make
# make install
# gtk-update-icon-cache /usr/share/icons/hicolor
```

Required development tools
--------------------------
* intltool
* autoconf
* make
* gcc-c++ / g++
* gtkmm3
* libdar >= 2.4.8
* attr  (libattr)
* gcrypt
* lzo2
* bz2 (bzib)
* libz

Example
----------
You can create a full backup of your home partition with dar by issuning the following command:
```
# dar -R /home/ -c /var/backup/home-$(date "+%Y%m%d")
```
In case of recovery just open the dar archive with gdar.

For a detailed documentation about the dar command-line tool visit:
http://dar.linux.free.fr/doc/Tutorial.html

Manual
------
Open a dar archive by clicking the open button.
When the archive is encrypted please activate the checkbox.
Afterwards enter the password and choose the chipher options.
Please be patient while the index (catalogue) of the archive is being read.
The symbol in front of the Name column indicates whether this is a file or a folder.
By double clicking on a folder you can move into this directory.
By clicking the Up button you can change into the parent directory.
You can also edit the path in the entry widget. Use the switch to display hidden files.
To extract a file or folder select it and click the button Extract and choose a destination.
The info button (light bulb) gives you a brief overview of the archive.

Known bugs
----------
* when running in a KDE environment the icons in the TreeView widget are hardly visible because of a gray background
