#!/bin/sh
set -e

echo "Activating feature 'cc65'"

cd /usr/src
git clone https://github.com/cc65/cc65.git
cd cc65
if [ "${CC65VERSION}" != "latest" ]; then
    git checkout tags/V${CC65VERSION} -b "tag-${CC65VERSION}"
fi
nice make -j2
make install PREFIX=/usr prefix=/usr
rm -rf /usr/src/cc65