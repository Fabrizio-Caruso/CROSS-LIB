# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.154.0/containers/alpine/.devcontainer/base.Dockerfile

# [Choice] Debian version: bookworm, buster, bullseye
ARG VARIANT="bookworm"

#FROM mcr.microsoft.com/vscode/devcontainers/base:${VARIANT}
FROM mcr.microsoft.com/vscode/devcontainers/python:3.12-bookworm
ARG CC65_VERSION="latest"

#RUN apk update && \
#    apk add --no-cache --virtual .build-deps git build-base

RUN mkdir -p /sdk
WORKDIR /sdk
ENV BUILD_SDCC=1
ENV BUILD_SDCC_HTTP=1
RUN git clone --recursive https://github.com/z88dk/z88dk.git && \
    apt update && \
    apt install -y build-essential bison flex libxml2-dev subversion zlib1g-dev m4 ragel re2c dos2unix texinfo texi2html gdb curl perl cpanminus ccache libboost-all-dev libmodern-perl-perl libyaml-perl liblocal-lib-perl libcapture-tiny-perl libpath-tiny-perl libtext-table-perl libdata-hexdump-perl libregexp-common-perl libclone-perl libfile-slurp-perl pkg-config libgmp3-dev && \
    cpanm --local-lib=~/perl5 App::Prove CPU::Z80::Assembler Data::Dump Data::HexDump File::Path List::Uniq Modern::Perl Object::Tiny::RW Regexp::Common Test::Harness Text::Diff Text::Table YAML::Tiny && \
    eval $(perl -I ~/perl5/lib/perl5/ -Mlocal::lib) && \
    cd z88dk && chmod 777 build.sh && ./build.sh
    #&& rm -R /sdk/z88dk
ENV PATH="$PATH:/sdk/z88dk/bin"

WORKDIR /sdk
RUN git clone https://github.com/cc65/cc65.git
WORKDIR /sdk/cc65
RUN echo 'Using CC65 Version: "${CC65_VERSION}"'

RUN if [ "${CC65_VERSION}" != "latest" ]; then \
    git checkout tags/${CC65_VERSION} -b "${CC65_VERSION}"; \
    fi
RUN nice make -j2
RUN make install PREFIX=/usr prefix=/usr

WORKDIR /sdk

RUN wget http://www.lwtools.ca/releases/lwtools/lwtools-4.22.tar.gz
RUN tar -xvf lwtools-4.22.tar.gz
WORKDIR /sdk/lwtools-4.22
RUN make && make install

WORKDIR /sdk
RUN wget http://perso.b2b2c.ca/~sarrazip/dev/cmoc-0.1.88.tar.gz
RUN tar -xvf cmoc-0.1.88.tar.gz
WORKDIR /sdk/cmoc-0.1.88
RUN ./configure && make && make install

WORKDIR /sdk