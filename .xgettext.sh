#!/bin/sh

xgettext --keyword=_ --package-name=makeit --package-version=0 -d makeit -p po/ -f po/files.in
mv po/makeit.po po/makeit.pot
