# CROSS-LIB SET-UP UNDER LINUX

We give here the procedure to install Cross-Lib and its dependencies under Linux.
In particular this procedure is the one I have used under LUBUNTU 21.10.
On other distributions the same procedure or a similar one can be used depending on how packages are handled.
Even under Windows with Cygwin many of these steps are similar but the way packages are installed is very different (i.e., using Cygwin's `setup.exe`).

We do not cover *all* supported compilers here. We only fully covered the fully supported ones under Linux.
In particular we describe how to install:
- `GCC` for native compilation
- `CC65` for MOS 6502-based targets
- `Z88DK` for Zilog 80, Intel 8080-based targets
- `CMOC` for Motorola 6809-based targets

We give some information on how to install some experimentally supported compilers:
- `ACK` for Intel 8088-based targets and other targets (partially supported and currently only used for one game)
- `LCC1802` for RCA COSMAC 1802-based targets (with only experimental support under Linux)

-----------------------------
## Python Installation

The whole tool-chain uses heavily Python scripts. So we need Python to do anything. 
Python 2.7 or Python 3.X can used.

Under any recent Ubuntu distribution, Python is trivially installed with:
```
sudo apt install python
```

-----------------------------
## Enable builds for the host 

Native builds can be performed as long as an ANSI C compiler and the `ncurses` libraries are installed.
By default Cross-Lib expects `gcc` to be the native compiler.
A different one can be used (including a non-native one, which would defeat the purpose of a native compiler).
In order to select a different compiler edit the `NATIVE_CC` variable in `src/makefiles.common/auxiliary/Makefile.compilers`.

The `gcc` compiler may be pre-installed. In not present, `gcc` is trivially installed with
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

So you can use Cross-Lib to build old and new games and to create new ones by using the `xl` command and by providing no target or equivalently the `ncurses` target.


To get started you can build some of the games by using `xl build [name of the game or test]`. 

Example:
The following commands will be three games for the native target:
```
xl build bomber
xl build snake
xl build horde
```



-----------------------------
## Enable builds for MOS 6502-based targets 

Nearly all MOS 6502-based targets supported by Cross-Lib rely on `CC65`. Very few others experimental targets use `VBCC`.

On very recent Ubuntu distributions we can install `CC65` by just:
```
sudo apt install cc65
```

In order to build disk images for the `Apple][` and `Apple//e` targets we need `java`, which we can install with:
```
sudo apt install openjdk-17-jre-headless
```


-----------------------------
## Enable builds for Motorola 6809-based targets

Cross-Lib relies on `CMOC` for all its supported Motorola 6809-targets. 
In order to install `CMOC` you can use build from its source code and follow its simple installation procedure.

First of all you need to download `LWTOOLS` (the linker) from http://www.lwtools.ca/, and install it with

```
make
sudo make install
```

Then you download `CMOC`'s source code from  http://perso.b2b2c.ca/~sarrazip/dev/cmoc.html and install it with:
```
apt install bison
apt install flex

configure
make
sudo make install
```
----------------------

## Enable builds for targets based on Zilog 80 and Intel 8080 

Cross-Lib relies on the `Z88DK` dev-kit for all its targets based on the Zilog 80, Intel 8080/5, and GBZ80.
`Z88DK` contains two separate cross-compilers: `SCCZ80` and `ZSDCC`. Using `SCCZ80` is enough to build most Cross-Lib targets.
So the installation of `ZSDCC` is not strictly necessary to build new games.

Here we describe how to download and install `Z88DK` from its source code.

### Z88DK dependencies
In order to be able to compile the source you have to install the following packages by running `apt install [list of packages]`:

```
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
```


### Installation from source code
In order to download `Z88DK`'s source code you can run:
```
wget http://nightly.z88dk.org/z88dk-latest.tgz
```
and then decompress it with:
```
tar -xzf z88dk-latest.tgz
```

You can now start the build process by using the `build.sh` script:
```
cd z88dk
export BUILD_SDCC=1
chmod 777 build.sh
./build.sh
```

In order to finalize the installation and to be able to use Z88DK from any location you need to set the following environment variables:
```
export PATH=${PATH}:${HOME}/z88dk/bin
export ZCCCFG=${HOME}/z88dk/lib/config
```

----------------------------------------------------------
## Enable builds for RCA COSMAC 1802-based targets

The build process for RCA COSMAC 1802-based targets relies on the `LCC1802` compiler.

This has not been tested, yet, under Linux.

Its installation procedure under Linux is described in:

https://github.com/bill2009/lcc1802

----------------------------------------------------------

## Enable builds for Intel 8088-based targets 

In order to use Cross-Lib for targets based on the Intel 8088 and some other 16 and 32-bit vintage targets you can use `ACK`.
Support for these targets is currently only experimental.

`ACK` can be downloaded from https://github.com/davidgiven/ack.

It depends only on `flex` and `yacc` or `bison` and it is installed with:
```
make
sudo make install
```


