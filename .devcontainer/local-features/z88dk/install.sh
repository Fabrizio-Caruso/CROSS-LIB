#!/bin/sh
set -e

echo "Activating feature 'z88dk'"

export BUILD_SDCC=1
export BUILD_SDCC_HTTP=1

mkdir -p /usr/src
cd /usr/src
git clone --depth 1 --recursive https://github.com/z88dk/z88dk.git 
apt update
#apt install -y build-essential bison flex libxml2-dev subversion zlib1g-dev m4 ragel re2c dos2unix texinfo texi2html gdb curl perl cpanminus ccache libboost-all-dev libmodern-perl-perl libyaml-perl liblocal-lib-perl libcapture-tiny-perl libpath-tiny-perl libtext-table-perl libdata-hexdump-perl libregexp-common-perl libclone-perl libfile-slurp-perl pkg-config libgmp3-dev 
#apt install -y build-essential bison flex subversion perl cpanminus libgmp-dev libxml2-dev pkg-config 

apt install -y bison flex libxml2-dev cpanminus libboost-all-dev perl ragel re2c ccache dos2unix texinfo texi2html libgmp3-dev libxml2-dev pkg-config 

#cpanm --local-lib=~/perl5 App::Prove CPU::Z80::Assembler Data::Dump Data::HexDump File::Path List::Uniq Modern::Perl Object::Tiny::RW Regexp::Common Test::Harness Text::Diff Text::Table YAML::Tiny
cpanm -l $HOME/perl5 --no-wget local::lib Template::Plugin::YAML
cd z88dk 
eval $(perl -I ~/perl5/lib/perl5/ -Mlocal::lib)
chmod 777 build.sh
./build.sh
make install PREFIX=/usr 
make install-clean bins-clean
apt-get remove --purge -y bison flex

unset BUILD_SDCC
unset BUILD_SDCC_HTTP
export PATH=$PATH:/usr/src/z88dk/bin
#rm -R /usr/sdk/z88dk