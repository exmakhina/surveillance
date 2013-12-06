#!/bin/sh -e
mkdir -p m4
mkdir -p config
touch NEWS README AUTHORS ChangeLog
autoreconf --force --install --verbose -I config -I m4
