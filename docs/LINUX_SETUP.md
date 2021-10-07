# LINUX INSTALLATION

We give here the procedure to install Cross-Lib and its dependencies under Linux.
In particular this procedure is the one used for LUBUNTU 21.10.
On other distributions the same procedure or a similar one can be used depending on how packages are handled.
Even under Windows with Cygwin many of these steps are similar but no `apt` commands are used. Use the `setup.exe` instead.

We do not cover *all* supported compilers here. We cover the fully supported ones under Linux.
In particular we describe how to install:
- `GCC` for native compilation
- `CC65` for MOS 6502 targets
- `Z88DK` for Zilog 80, Intel 8080/5 and GBZ80 targets
- `CMOC` for Motorola 6809 targets
- `ACK` for Intel 8088 and other targets (partially supported and currently only used for one game)

-----------------------------
## Python Installation

The tool-chain uses heavily Python scripts. So we need Python to do anything.

Python is trivially installed with:
```
sudo apt install python
```

-----------------------------
## Building native (for the host) 

Native builds can be performed as long as an ANSI C compiler and the `ncurses` libraries are installed.
By default Cross-Lib expects `gcc` to be the native compiler.
A different one can be used (including a non-native one, which would defeat the purpose of a native compiler).
In order to select a different compiler edit the `NATIVE_CC` variable in `src/makefiles.common/auxiliary/Makefile.compilers`.

If not pre-installed, `gcc` is trivially installed with
```
sudo apt install gcc
```

The `ncurses` libraries can be installed with:
```
sudo apt install libncurses5-dev libncursesw5-dev
```

The native target is the default target for the script command `xl build`.
This makes it possible to try and debug the code natively if desired. No emulator and no cross-compiler is necessary to start using Cross-Lib.
Currently the native target is just an ncurses-enabled terminal target. So no graphics. Only ASCII characters will be displayed.

In order to build some of the games you can just do something `xl build [name of the game or test]`. 

Example:
The following commands will be three games for the native t:
```
xl build bomber
xl build snake
xl build horde
```



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

## INTEL 8088 and several vintage Linux systems with ACK

https://github.com/davidgiven/ack

(It requies flex and yacc/bison)

make
sudo make install


