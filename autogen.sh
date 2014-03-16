#!/bin/sh

aclocal
autoconf
automake -a
autoreconf
intltoolize --force
