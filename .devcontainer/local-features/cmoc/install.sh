#!/bin/sh
set -e

echo "Activating feature 'cmoc'"

mkdir -p /usr/src
cd /usr/src

wget http://www.lwtools.ca/releases/lwtools/lwtools-4.23.tar.gz
tar -xvf lwtools-4.23.tar.gz
cd lwtools-4.23
make
make install

# apk add --no-cache -t .build_deps bison flex

apt-get update
apt-get install -y bison flex
wget http://perso.b2b2c.ca/~sarrazip/dev/cmoc-0.1.88.tar.gz
tar -xvf cmoc-0.1.88.tar.gz
cd cmoc-0.1.88
./configure
make
make install
apt remove --purge -y bison flex