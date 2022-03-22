#!/usr/bin/perl

# Convert a binary and a map file to a C file

use bytes;
use strict;
use File::Spec;

my ($prefix, $bin, $map, $c, $h, $i) = @ARGV;

if (!defined($h)) {
    die "Usage: $0 prefix file.bin file.map file.c file.h [common.h]\n";
}

open(BIN, '<:raw', $bin) or die;
open(MAP, '<', $map) or die;
open(C, '>', $c) or die;
open(H, '>', $h) or die;

my $bindata;
my $binlen = read(BIN, $bindata, 65536);

close(BIN);

my %syms = ();
while (defined(my $line = <MAP>)) {
    if ($line =~ /^(\S+)\s*\=\s*\$?([0-9a-f]+)/i) {
	$syms{$1} = hex $2;
    }
}
close(MAP);

# Symbols sorted in order of appearance
my @ssym = sort { $syms{$a} <=> $syms{$b} } keys(%syms);

my $hguard = "BIN_\U$h";
$hguard =~ s/[^A-Z0-9_]/_/g;

print H "#ifndef $hguard\n";
print H "#define $hguard 1\n\n";

print H "extern unsigned char ${prefix}_bin[${binlen}];\n\n";

foreach my $s (@ssym) {
    printf H "#define %-31s 0x%04x\n", $prefix.'_SYM_'.$s, $syms{$s};
}

print H "\n#endif /* $hguard */\n";

close(H);

print C "#include \"$h\"\n\n";

print C "unsigned char ${prefix}_bin[${binlen}] = {\n";

my $rlen = 0;
my $pfx = "\t";

my @bin = unpack("C*", $bindata);

my $pfx = "\t";
my $rlen = 0;
for (my $i = 0; $i < $binlen; $i++) {
    printf C '%s0x%02x', $pfx, shift(@bin);
    $rlen++;
    if ($rlen >= 8) {
	$pfx = ",\n\t";
	$rlen = 0;
    } else {
	$pfx = ", ";
    }
}
printf C "\n};\n";

close(C);

if ($i) {
    # See if we need to append this to the common include file
    my ($ivol,$idir,$ifile) = File::Spec->splitpath($i);
    my $habs = File::Spec->rel2abs($h);
    my $found = 0;
    $idir = File::Spec->catdir($ivol, $idir) if ($ivol ne '');

    open(I, '+>>', $i) or die;
    while (defined(my $line = <I>)) {
	chomp $line;
	if ($line =~ /^\s*\#\s*include\s+\"(.*)\"\s*$/) {
	    my $iabs = File::Spec->rel2abs($1, $idir);
	    $found = 1 if ($habs eq $iabs);
	}
    }

    if (!$found) {
	# Need to add this header to the common include file
	printf I "#include \"%s\"\n", File::Spec->abs2rel($habs, $idir);
    }
    close(I);
}

exit(0);
