## NO LOCAL INSTALLATION

Having a Dockerfile and a devcontainer configuration means that anyone with a GitHub account can just clone this repository and run it with GitHub CodeSpaces (free of charge with some restrictions): https://github.com/codespaces.

-------------------------------------------

## LOCAL DOKCER WITH A PRE-BUILD DOCKER IMAGE

You can pull and run the latest pre-build image with
```
docker run -it -v {cross-lib_location}:/usr/src/cross-lib ghcr.io/cross-lib/devcontainers/base:1.0.5-ubuntu
```
where {cross-lib_location} could be something like: *c:\sources\cross-lib*.

Once in the docker container, you can enter the *src* directory with *cd /usr/src/cross-lib/src* and 
use the *xl* script, e.g., *./xl stinger cc65*.

Or just pull  the pre-build image with
```
docker pull ghcr.io/cross-lib/devcontainers/base:1.0.5-ubuntu
```

An older image can be pulled here
```
docker pull fabriziocaruso:crosslib:0.1
```

and follow the same instructions as for docker image built from a dockerfile.

-------------------------------------------

## LOCAL DOCKER WITH THE DOCKERFILE

The dockerized version of Cross-Lib can used by 
- cloning this repository into a directory (let us refer to it as `<local_cross-lib>`);
- building the docker image that contains the dependencies by running the command 

```docker build . -t crosslib:<version>```;

from the `/CROSS-LIB/.devcontainer` directory.

- running the docker container and mapping the cloned repository into a directory (e.g. `/workspaces/crosslib`) in the container with

```docker run -it -v <local_cross-lib>:/workspaces/crosslib --env-file <local_cross-lib>/.devcontainer/container.env crosslib:<version> bash```
- (once in the docker container) by entering `workspaces/crosslib/src` and running `xl` commands.

-------------------------------------------

## LOCAL NON-DOCKER INSTALLATION - PREREQUISITES

Cross-Lib is meant to be used under a POSIX environment (Windows+Cygwin, Linux, etc.).

In order to be able to build the games and tests on all `default targets` you need:
- GNU `make`
- `python` (2.x or 3.x)
- `gcc` and `ncurses` (necessary only for native builds)
- cross-compilers (necessary to build for vintage targets)
- `java` only to build disk images for the Apple//e, Apple\]\[ and the Agat 7/9 targets
- `perl` only to build cassette images for the CoCo 1/2 and Dragon 32/64 targets

For the compilers to work, you may need to add the location of their binaries to the `$PATH` environment variable and to add the execution rights to those binaries.
If the compiler's binary cannot be found, you may have to set their paths in: `src\makefiles.common\auxiliary/Makefile_compilers`, e.g., `Z88DK_PATH`, `LCC1802_PATH`, etc..

For more details on the prerequisites we refer to: 

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/PREREQUISITES.md


-------------------------------------------
## LOCAL NON-DOCKER INSTALLATION OF THE TOOL-CHAIN

The tool-chain Cross-Lib can be installed on different POSIX-compliant environments.
It has been tested on:
- Windows (7, 10) + `Cygwin`
- Windows Subsystem for Linux
- MacOS X
- Linux Ubuntu 18.04 under the Windows Subsystem
- Linux (KUbuntu 18.04, Lubuntu 21.10) 
- FreeBSD 13.0

Cross-Lib itself does not require any installation. It is just a source library and scripts that is ready to be used as is, by just downloading or cloning the content of this repository.
For example you can do it by:

```
git clone https://github.com/Fabrizio-Caruso/CROSS-LIB.git
```

In order to use Cross-Lib you will need to install the prerequisites described in the previous section. 
The way these prerequisites are installed vary depending on the environment.

### Under Windows + Cygwin
For Windows + Cygwin the tested and verified installation procedure uses:
- `Cygwin` setup to install `gcc`, `make`, `python`, `java`
- `CC65` is installed under Windows by just downloading and decompressing its snapshot (https://sourceforge.net/projects/cc65/).
- `Z88DK` is installed under Windows by downloading and decompressing its latest nightly build (http://nightly.z88dk.org/).
- `CMOC` and `ACK` are installed from sources. Look at the installation procedure under Linux for the details.
- `LCC1802` is installed under Windows by simply decompressing its released binary version (https://github.com/bill2009/lcc1802/releases).

### Under Linux
For Linux (native or under the Windows Subsystem), the tested and verified procedure both standard repositories and installation from sources.

A detailed procedure for recent Ubuntu distributions is described here: 

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/LINUX_SETUP.md



### ACKNOWLEDGEMENTS


Thanks to Alex Thissen, a version of Cross-Lib with (Z88DK, CC65, CMOC and native compilers) is available as a Docker image (to be built by using the provided Dockerfile) with a devcontainer configuration.






