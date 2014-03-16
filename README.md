gdar 
====

A graphical user interface to brows and extract dar archives

The GUI is written in gtkmm, as backend libdar is used.
The state of development is beta, so feel free to report bugs.

prepare when cloning from git
-----------------------------
`$ ./autogen.sh`

build
-----
```
$ ./configure [--enable-libdar64] [--enable-date_s]
$ make
# make install
```

required development tools
--------------------------
* intltool
* autoconf
* make
* gcc-c++ / g++
* gtkmm3
* libdar
* boost
* attr  (libattr)
* gcrypt
* lzo2
* bz2 (bzib)
* libz

test it
-------
create a backup:
`# dar -R /home/ -c /var/backup/home-`date "+%Y%m%d"``
and brows it with gdar

manual
------
Open a dar archive by clicking the open button.
Please be patient while the index (catalogue) of the archive is being read.
The symbol in front of the Name column indicates whether this is a file or a folder.
By double clicking on a folder you can move into this directory.
By clicking the Up button you can change into the parent directory.
Use the switch to display hidden files.
To extract a file or folder select it and click the button Extract and choose a destination.
The info button (light bulb) should give you a brief overview of the archvie. (todo)

known bugs
----------
* when running in a KDE environment the icons in the TreeView widget are hardly visible because of a gray background
* building against libdar64 v5.3.0 (dar-2.4.8) fails, please use libdar in 32Bit mode

todo
----
* support encrypted archives
* support editing the path in the Entry widget
* implement the info button
