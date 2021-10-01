#LUBUNTU 21.10 / KUBUNTU 20.04

-----------------------------
## PYTHON
sudo apt install python

-----------------------------
## NATIVE builds with GCC+NCURSES

### GCC
sudo apt install gcc

### NCURSES
sudo apt install libncurses5-dev libncursesw5-dev

-----------------------------
## MOS 6502 builds with CC65

### JAVA
sudo apt install openjdk-17-jre-headless

### CC65
sudo apt install cc65

-----------------------------
## MOTOROLA 6809 builds with CMOC

### LWTOOLS
http://www.lwtools.ca/

make
sudo make install

### CMOC Source code
http://perso.b2b2c.ca/~sarrazip/dev/cmoc.html

apt install bison
apt install flex

configure
make
sudo make install

----------------------

## ZILOG 80 and INTEL 8080 builds with Z88DK

(tested with: http://nightly.z88dk.org/z88dk-20211001-e12622c-18689.tgz)

wget http://nightly.z88dk.org/z88dk-latest.tgz
tar -xzf z88dk-latest.tgz

apt install
    build-essential
    dos2unix
    libboost-all-dev
    texinfo
    texi2html
    libxml2-dev
    subversion
    bison
    flex
    zlib1g-dev
    m4

cd z88dk
export BUILD_SDCC=1
chmod 777 build.sh
./build.sh

export PATH=${PATH}:${HOME}/z88dk/bin
export ZCCCFG=${HOME}/z88dk/lib/config

----------------------------------------------------------

## INTEL 8080 and several vintage Linux systems with ACK

https://github.com/davidgiven/ack

(It requies flex and yacc/bison)

make
sudo make install


