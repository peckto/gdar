gdar 
====

A graphical user interface to brows and extract dar archives

The GUI is written in gtkmm, as backend libdar is used. <br>
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
* attr  (libattr)
* gcrypt
* lzo2
* bz2 (bzib)
* libz

try it out
----------
You can create a full backup of your home partition with dar by issuning the following command:<br>
`# dar -R /home/ -c /var/backup/home-$(date "+%Y%m%d")` <br>
In case of recovery just open the dar archive with gdar. <br>
<br>
For a detailed documentation about the dar command-line tool visit: <br>
http://dar.linux.free.fr/doc/Tutorial.html

manual
------
Open a dar archive by clicking the open button. <br>
Please be patient while the index (catalogue) of the archive is being read. <br>
The symbol in front of the Name column indicates whether this is a file or a folder. <br>
By double clicking on a folder you can move into this directory. <br>
By clicking the Up button you can change into the parent directory. <br>
Use the switch to display hidden files. <br>
To extract a file or folder select it and click the button Extract and choose a destination. <br>
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
