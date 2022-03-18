#
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#
#    W   W    A    RRRR   N   N   III  N   N  III  N   N   GGG
#    W   W   A A   R   R  NN  N    I   NN  N   I   NN  N  G   G
#    W   W  A   A  RRRR   N N N    I   N N N   I   N N N  G
#    W W W  AAAAA  R R    N  NN    I   N  NN   I   N  NN  G  GG
#    W W W  A   A  R  R   N   N    I   N   N   I   N   N  G   G
#     W W   A   A  R   R  N   N   III  N   N  III  N   N   GGG
#
# Warning: DO NOT send patches which fix this file. IT IS NOT the original
# source file. This file is GENERATED from the Aegis repository file manifest.
# If you find a bug in this file, it could well be an Aegis bug.
#
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#
Summary: srecord
Name: srecord
Version: 1.64.D001
Release: 1
License: GPL
Group: Development/Tools
Source: http://srecord.sourceforge.net/%{name}-%{version}.tar.gz
URL: http://srecord.sourceforge.net/
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildPrereq: boost-devel, diffutils, ghostscript, groff, libgcrypt-devel
BuildPrereq: libtool, sharutils

%description
This package contains the shared libraries for programs that manipulate EPROM
load files.

%package bin
Summary: srecord programs

%description bin
The srecord package is a collection of powerful tools for manipulating EPROM
load files. It reads and writes numerous EPROM file formats, and can perform
many different manipulations.

%package libs0
Summary: srecord libraries
Group: Development/Tools

%description libs0
This package contains the shared libraries for programs that manipulate EPROM
load files.

%package devel
Summary: srecord development files
Group: Development/Tools
Requires: srecord-libs%{?_isa} = %{?epoch:%{epoch}:}%{version}-%{release}

%description devel
This package contains static libraries and header files for compiling programs
that manipulate EPROM load files.

%post
/sbin/ldconfig

%postun
/sbin/ldconfig


%prep
%setup -q


%build
%configure --sysconfdir=/etc --prefix=%{_prefix} --mandir=%{_mandir}
make


%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install
rm -f $RPM_BUILD_ROOT/usr/lib/*.la


%check
make sure


%clean
rm -rf $RPM_BUILD_ROOT


%files bin
%defattr (-,root,root,-)
%doc LICENSE BUILDING README
%doc %{_datarootdir}/doc/srecord/BUILDING.pdf
%doc %{_datarootdir}/doc/srecord/README.pdf
%doc %{_datarootdir}/doc/srecord/change_log.pdf
%doc %{_datarootdir}/doc/srecord/reference.pdf
%{_bindir}/srec_cat
%{_bindir}/srec_cmp
%{_bindir}/srec_info
%{_mandir}/man1/srec_cat.1
%{_mandir}/man1/srec_cmp.1
%{_mandir}/man1/srec_examples.1
%{_mandir}/man1/srec_info.1
%{_mandir}/man1/srec_input.1
%{_mandir}/man1/srec_license.1
%{_mandir}/man5/srec_aomf.5
%{_mandir}/man5/srec_ascii_hex.5
%{_mandir}/man5/srec_atmel_generic.5
%{_mandir}/man5/srec_binary.5
%{_mandir}/man5/srec_brecord.5
%{_mandir}/man5/srec_coe.5
%{_mandir}/man5/srec_cosmac.5
%{_mandir}/man5/srec_dec_binary.5
%{_mandir}/man5/srec_emon52.5
%{_mandir}/man5/srec_fairchild.5
%{_mandir}/man5/srec_fastload.5
%{_mandir}/man5/srec_formatted_binary.5
%{_mandir}/man5/srec_forth.5
%{_mandir}/man5/srec_fpc.5
%{_mandir}/man5/srec_idt.5
%{_mandir}/man5/srec_intel.5
%{_mandir}/man5/srec_intel16.5
%{_mandir}/man5/srec_logisim.5
%{_mandir}/man5/srec_mem.5
%{_mandir}/man5/srec_mif.5
%{_mandir}/man5/srec_mips_flash.5
%{_mandir}/man5/srec_mos_tech.5
%{_mandir}/man5/srec_motorola.5
%{_mandir}/man5/srec_msbin.5
%{_mandir}/man5/srec_needham.5
%{_mandir}/man5/srec_os65v.5
%{_mandir}/man5/srec_ppb.5
%{_mandir}/man5/srec_ppx.5
%{_mandir}/man5/srec_signetics.5
%{_mandir}/man5/srec_spasm.5
%{_mandir}/man5/srec_spectrum.5
%{_mandir}/man5/srec_stewie.5
%{_mandir}/man5/srec_tektronix.5
%{_mandir}/man5/srec_tektronix_extended.5
%{_mandir}/man5/srec_ti_tagged.5
%{_mandir}/man5/srec_ti_tagged_16.5
%{_mandir}/man5/srec_ti_txt.5
%{_mandir}/man5/srec_trs80.5
%{_mandir}/man5/srec_vmem.5
%{_mandir}/man5/srec_wilson.5


%files libs
%defattr (-,root,root,-)
%{_libdir}/libsrecord.so.*


%files devel
%defattr (-,root,root,-)
%{_includedir}/srecord/adler16.h
%{_includedir}/srecord/adler32.h
%{_includedir}/srecord/crc16.h
%{_includedir}/srecord/crc32.h
%{_includedir}/srecord/defcon.h
%{_includedir}/srecord/endian.h
%{_includedir}/srecord/fletcher16.h
%{_includedir}/srecord/fletcher32.h
%{_includedir}/srecord/format_printf.h
%{_includedir}/srecord/input.h
%{_includedir}/srecord/input/catenate.h
%{_includedir}/srecord/input/file.h
%{_includedir}/srecord/input/file/aomf.h
%{_includedir}/srecord/input/file/ascii_hex.h
%{_includedir}/srecord/input/file/atmel_generic.h
%{_includedir}/srecord/input/file/binary.h
%{_includedir}/srecord/input/file/brecord.h
%{_includedir}/srecord/input/file/cosmac.h
%{_includedir}/srecord/input/file/dec_binary.h
%{_includedir}/srecord/input/file/emon52.h
%{_includedir}/srecord/input/file/fairchild.h
%{_includedir}/srecord/input/file/fastload.h
%{_includedir}/srecord/input/file/formatted_binary.h
%{_includedir}/srecord/input/file/four_packed_code.h
%{_includedir}/srecord/input/file/intel.h
%{_includedir}/srecord/input/file/logisim.h
%{_includedir}/srecord/input/file/mif.h
%{_includedir}/srecord/input/file/mips_flash.h
%{_includedir}/srecord/input/file/mos_tech.h
%{_includedir}/srecord/input/file/msbin.h
%{_includedir}/srecord/input/file/needham.h
%{_includedir}/srecord/input/file/os65v.h
%{_includedir}/srecord/input/file/ppb.h
%{_includedir}/srecord/input/file/ppx.h
%{_includedir}/srecord/input/file/signetics.h
%{_includedir}/srecord/input/file/spasm.h
%{_includedir}/srecord/input/file/spectrum.h
%{_includedir}/srecord/input/file/stewie.h
%{_includedir}/srecord/input/file/tektronix.h
%{_includedir}/srecord/input/file/tektronix_extended.h
%{_includedir}/srecord/input/file/ti_tagged.h
%{_includedir}/srecord/input/file/ti_tagged_16.h
%{_includedir}/srecord/input/file/ti_txt.h
%{_includedir}/srecord/input/file/trs80.h
%{_includedir}/srecord/input/file/vmem.h
%{_includedir}/srecord/input/file/wilson.h
%{_includedir}/srecord/input/filter.h
%{_includedir}/srecord/input/filter/and.h
%{_includedir}/srecord/input/filter/bitrev.h
%{_includedir}/srecord/input/filter/byte_swap.h
%{_includedir}/srecord/input/filter/checksum.h
%{_includedir}/srecord/input/filter/checksum/bitnot.h
%{_includedir}/srecord/input/filter/checksum/negative.h
%{_includedir}/srecord/input/filter/checksum/positive.h
%{_includedir}/srecord/input/filter/crop.h
%{_includedir}/srecord/input/filter/fill.h
%{_includedir}/srecord/input/filter/interval.h
%{_includedir}/srecord/input/filter/interval/length.h
%{_includedir}/srecord/input/filter/interval/maximum.h
%{_includedir}/srecord/input/filter/interval/minimum.h
%{_includedir}/srecord/input/filter/message.h
%{_includedir}/srecord/input/filter/message/adler16.h
%{_includedir}/srecord/input/filter/message/adler32.h
%{_includedir}/srecord/input/filter/message/crc16.h
%{_includedir}/srecord/input/filter/message/crc32.h
%{_includedir}/srecord/input/filter/message/fletcher16.h
%{_includedir}/srecord/input/filter/message/fletcher32.h
%{_includedir}/srecord/input/filter/message/gcrypt.h
%{_includedir}/srecord/input/filter/not.h
%{_includedir}/srecord/input/filter/offset.h
%{_includedir}/srecord/input/filter/or.h
%{_includedir}/srecord/input/filter/random_fill.h
%{_includedir}/srecord/input/filter/sequence.h
%{_includedir}/srecord/input/filter/split.h
%{_includedir}/srecord/input/filter/unfill.h
%{_includedir}/srecord/input/filter/unsplit.h
%{_includedir}/srecord/input/filter/xor.h
%{_includedir}/srecord/input/generator.h
%{_includedir}/srecord/input/generator/constant.h
%{_includedir}/srecord/input/generator/random.h
%{_includedir}/srecord/input/generator/repeat.h
%{_includedir}/srecord/interval.h
%{_includedir}/srecord/memory.h
%{_includedir}/srecord/memory/chunk.h
%{_includedir}/srecord/memory/walker.h
%{_includedir}/srecord/memory/walker/compare.h
%{_includedir}/srecord/memory/walker/continuity.h
%{_includedir}/srecord/memory/walker/gcrypt.h
%{_includedir}/srecord/memory/walker/writer.h
%{_includedir}/srecord/output.h
%{_includedir}/srecord/output/file.h
%{_includedir}/srecord/output/file/aomf.h
%{_includedir}/srecord/output/file/ascii_hex.h
%{_includedir}/srecord/output/file/asm.h
%{_includedir}/srecord/output/file/atmel_generic.h
%{_includedir}/srecord/output/file/basic.h
%{_includedir}/srecord/output/file/binary.h
%{_includedir}/srecord/output/file/brecord.h
%{_includedir}/srecord/output/file/c.h
%{_includedir}/srecord/output/file/cosmac.h
%{_includedir}/srecord/output/file/dec_binary.h
%{_includedir}/srecord/output/file/emon52.h
%{_includedir}/srecord/output/file/fairchild.h
%{_includedir}/srecord/output/file/fastload.h
%{_includedir}/srecord/output/file/formatted_binary.h
%{_includedir}/srecord/output/file/forth.h
%{_includedir}/srecord/output/file/four_packed_code.h
%{_includedir}/srecord/output/file/hexdump.h
%{_includedir}/srecord/output/file/intel.h
%{_includedir}/srecord/output/file/intel16.h
%{_includedir}/srecord/output/file/logisim.h
%{_includedir}/srecord/output/file/mif.h
%{_includedir}/srecord/output/file/mips_flash.h
%{_includedir}/srecord/output/file/mos_tech.h
%{_includedir}/srecord/output/file/msbin.h
%{_includedir}/srecord/output/file/needham.h
%{_includedir}/srecord/output/file/os65v.h
%{_includedir}/srecord/output/file/ppb.h
%{_includedir}/srecord/output/file/ppx.h
%{_includedir}/srecord/output/file/signetics.h
%{_includedir}/srecord/output/file/spasm.h
%{_includedir}/srecord/output/file/spectrum.h
%{_includedir}/srecord/output/file/stewie.h
%{_includedir}/srecord/output/file/tektronix.h
%{_includedir}/srecord/output/file/tektronix_extended.h
%{_includedir}/srecord/output/file/ti_tagged.h
%{_includedir}/srecord/output/file/ti_tagged_16.h
%{_includedir}/srecord/output/file/ti_txt.h
%{_includedir}/srecord/output/file/trs80.h
%{_includedir}/srecord/output/file/vhdl.h
%{_includedir}/srecord/output/file/vmem.h
%{_includedir}/srecord/output/file/wilson.h
%{_includedir}/srecord/quit.h
%{_includedir}/srecord/quit/exception.h
%{_includedir}/srecord/quit/normal.h
%{_includedir}/srecord/quit/prefix.h
%{_includedir}/srecord/record.h
%{_includedir}/srecord/srecord.h
%{_includedir}/srecord/string.h
%{_libdir}/pkgconfig/srecord.pc
%{_mandir}/man3/srecord.3
%{_mandir}/man3/srecord_license.3
%{_libdir}/libsrecord.a
%{_libdir}/libsrecord.so
