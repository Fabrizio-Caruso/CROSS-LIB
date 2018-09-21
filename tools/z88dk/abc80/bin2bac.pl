#!/usr/bin/perl

use bytes;

# Pad out to the next block.  $final = 1 if and only if this is the last block.
sub genpad80($$) {
    my($left, $final) = @_;

    my $o = ($final ? "\x01" : "\x00") . ("\x00" x $$left);
    $$left = 252;		# Bytes left in block minus end marker

    return $o;
}

# Generate a line in a .bac file; pad the previous block if necessary
sub bacstmt80($$$) {
    my($line, $left, $data) = @_;
    my $l = length($data) + 4;
    my $d = pack("Cv", $l, $line) . $data . "\x0d";

    if ($l > $$left) {
	$d = genpad80($left,0).$d;
    }

    $$left -= $l;
    return $d;
}

# Take a list of relocations as 16-bit numbers and convert them
# to packed form.  Returns a list of three elements: the adjusted
# data block, the packed relocations, and the final
# position of the address pointer.
sub pack_relocs($$) {
    my($data, $bin_relocs) = @_;
    my @relocs = sort { $a <=> $b } unpack('v*', $bin_relocs);
    my $l = length($data);

    my $ptr = 0;		# Address pointer
    my $prels = '';		# Packed relocations
    foreach my $rel ( @relocs ) {
	if ($rel > $l-1) {
	    die sprintf("$0: relocation past end of data: %d (0x%x)\n",
			$rel, $rel);
	}

	my $delta = $rel - $ptr;

	if ($delta < 0) {
	    die "$0: negative relocation jump: $delta\n";
	} elsif ($delta <= 0x7f) {
	    $prels .= pack('C', $delta);
	} elsif ($delta <= 0x7eff) {
	    # This is *bigendian*, with an offset of 0x8100
	    $prels .= pack('n', $delta + 0x8100);
	} else {
	    die sprintf("$0: impossibly large relocation jump: %d (0x%x)\n",
			$delta, $delta);
	}

	# Adjust data bytes to make relocations self-relative
	my $a = unpack("v", substr($data, $rel, 2));
	substr($data, $rel, 2) = pack("v", $a - $rel);

	$ptr = $rel + 2;
    }

    $prels .= pack('C', 0x80);	# Relocation end marker

    return ($data, $prels, $ptr);
}

sub makebac80($$$$) {
    my($data, $addr, $entrypt, $relocs) = @_;

    my $bld;

    if (defined($addr)) {
	# <bacldr.asm code> - fixed load address

	# 0x00C9 is the address for END in all ABC80 BASIC interpreters
	$entrypt = 0x00c9 unless(defined($entrypt));

	$bld  = "\x2a\x1c\xfe\x06\x00\x4e\x09\x11";
	$bld .= pack('v', $addr);	# 16-bit load address
	$bld .= "\x06\x00\x7e\xd6\x08\xda";
	$bld .= pack('v', $entrypt);	# 16-bit entry point
	$bld .= "\x0e\x06\x09\x4f\xed\xb0\x23\x23\x18\xf0";
    } else {
	# <bacldrr.asm> - relocatable data loaded at BOFA

	# Default to entry = BOFA
	$entrypt = 0 unless(defined($entrypt));

	# Raw data length, must be recorded before pack_relocs()
	my $data_len = length($data);

	my $relptr;
	($data, $relocs, $relptr) = pack_relocs($data, $relocs);
	$data .= $reldata;

	$bld  = "\x01\xc9\x00\xc5\x2a\x1c\xfe\xe5\x5d\x54\x4e\x09";
	$bld .= "\x7e\xd6\x08\x38\x0a\x0e\x06\x09\x4f\xed\xb0";
	$bld .= "\x23\x23\x18\xf1\x21";
	# 16-bit length of data not including relocations
	$bld .= pack('v', $data_len);
	$bld .= "\xd1\x19\xeb\x1a\x13\xfe\x80\x30\x0e\x4f\x06\x00";
	$bld .= "\x09\x7d\x86\x77\x7c\x23\x8e\x77\x23\x18\xec";
	$bld .= "\xd6\x81\x38\x06\x47\x1a\x13\x4f\x18\xeb\x01";
	# 16-bit offset from final relocation to entry point
	$bld .= pack('v', $entrypt - $relptr);
	$bld .= "\x09\xe9";
    }

    my $q  = "\x82";		# Output (program start marker)
    my $left = 251;		# Bytes left in block
    my $r = 0;			# Last emitted line number

    # Address 65054 is EOFA
    # 1 PRINT CALL(PEEK(65054%)+SWAP%(PEEK(65055%))-<loader offset>)
    # Use PRINT here rather than the traditional Z%=, because that
    # way we don't create any variables on the heap. As a side benefit,
    # it is actually shorter. Since the CALL() never returns, it will
    # never actually print anything anyway.
    $q .= bacstmt80(++$r, \$left,
		    "\x84\xc7\x1e\xfe\xce\x36\xc7\x1f\xfe\xce\x34\xf5\xc7".
		    pack("v", length($bld)+2). # +2 for statement trailer
		    "\xf8\xce\x3a\xba");

    my $i = 0;
    my $dl = length($data);
    while ($i < $dl) {
	my $l = $dl - $i;

	# 8 byte overhead:
	# - 1 byte instruction length
	# - 2 bytes line number
	# - 2 bytes CB 22 [string expression in " quotes]
	# - 1 byte data length
	#     <data bytes>
	# - 1 byte BB (drop expression)
	# - 1 byte <CR> (end of statement)

	$q .= genpad(\$left,0) if ($left <= 8);
	$l = $left-8 if ($l > $left-8);

	# String expression + data + return
	$q .= bacstmt(++$r, \$left,
		      "\xcb\"" . pack("C", $l) . substr($data,$i,$l)."\xbb");

	$i += $l;
	$addr += $l;
    }

    # Terminal STOP statement (should obviously never execute)
    $q .= bacstmt(++$r, \$left, "\x86\x89"); # STOP

    # Loader code (string expression)
    $q .= bacstmt(++$r, \$left, "\xcb\"".pack("C",length($bld)).$bld."\xbb");
    $q .= genpad(\$left,1);

    return $q;
}

sub makebac800($$$$) {
    my($data, $entry, $relocs, $reserve) = @_;

    (undef, $relocs, undef) = pack_relocs($data, $relocs);
    # For ABC800 we don't use the adjusted data

    # The first relocation needs to be adjusted to account for the
    # size of the relocations themselves
    my $r1 = unpack("C", $relocs);
    if ($r1 != 0x80) {		# No relocations whatsoever?
	my $rlen = 1;
	if ($r1 > 0x7f) {
	    $r1 = unpack("n", $relocs) - 0x8100;
	    $rlen = 2;
	}

	$r1 += 2 + length($relocs);
	if ($r1 > 0x7f) {
	    if ($rlen == 1) {
		$r1++;
	    }
	    substr($relocs, 0, $rlen) = pack("n", $r1 + 0x8100);
	} else {
	    substr($relocs, 0, $rlen) = pack("C", $r1);
	}
    }

    my $bhdr;
    # See bac800.asm
    $bhdr  = "\x8f\x00\x01\x00";
    $bhdr .= pack("v", 0x5e + length($relocs) + length($data));
    $bhdr .= pack("v*", 0, 0, 0, $reserve, 0, 0, 0);
    $bhdr .= "\x82\x02\x00\x92\xe8\xd1\x19\xfe\x9c\xd1\x17\xb8\xa0";
    $bhdr .= "\x01" . pack("v", 0xffca - length($relocs));
    $bhdr .= "\x7d\x91\x4f\x7c\x98\x47\xb1\x28\x25\xe5\x11\x34\x00";
    $bhdr .= "\x19\xd1\x7e\xfe\x80\x30\x10\x23\x83\x5f\x30\x01\x14";
    $bhdr .= "\x1a\x81\x12\x13\x1a\x88\x12\x13\x18\xeb\xd6\x81\x38\x06";
    $bhdr .= "\x23\x82\x57\x7e\x18\xe6";
    $bhdr .= "\xc3" . pack("v", $entry);
    $bhdr .= "\x01\x2f";

    $data = $bhdr . $relocs . $data;

    # Trailer = "no long variable names"
    $data .= "\x00\x00\x00\x00\xff\xfb\xff";

    # Pad to the end of a binary sector
    $data .= ("\0" x (253 - (((length($data)-1) % 253) + 1)));

    return $data;
}

sub intval($) {
    my($s) = @_;

    $s =~ s/[\s_]//g;

    return 0 if ($s eq '');
    return undef if ($s !~ /^(0[0-7]*|0x[0-9a-f]+|[0-9]+)$/i);
    return ($s =~ /^0/) ? oct $s : $s+0;
}

my $abc800 = 0;
my $reserve = 0;

while ($ARGV[0] =~ /^-/) {
    my $opt = shift;
    if ($opt eq '-80') {
	$abc000 = 0;
    } elsif ($opt eq '-800') {
	$abc800 = 1;
    } elsif ($opt eq '-reserve') {
	$reserve = intval(shift);
    } else {
	die "$0: Unknown option: $opt\n";
    }
}

($file, $org, $entry, $entryname) = @ARGV;

if (!defined($file)) {
    die "Usage: $0 [-80|-800] [-reserve bytes]\n".
	"       inputfile {load_addr|relocfile} [entrypoint|deffile [entryname]]\n";
}

my $relocfile = $org;
$org = intval($org);
undef $relocfile if (defined($org));

my $entryfile = $entry;
$entry = intval($entry);
if (!defined($entry)) {
    $entryname = '_start' unless(defined($entryname));
    undef $entry;

    open(DEF, '<', $entryfile) or die "$0: $entryfile: $!\n";
    while ($line = <DEF>) {
	chomp $line;
	@l = split(/\s+/, $line);
	if ($l[0] =~ /^DEFC$/i && $l[1] eq $entryname && $l[2] eq '=' &&
	    $l[3] =~ /^\$([0-9a-f]+)$/i) {
	    $entry = hex $1;
	    last;
	}
    }
    close(DEF);
    if (!defined($entry)) {
	die "$0: symbol $entryname not found in file $entryfile\n";
    }
}

open(FILE, '<:raw', $file) or die "$0: $file: $!\n";
read(FILE, $dd, 65536);
close(FILE);

if (defined($relocfile)) {
    if (defined($org)) {
	die "$0: cannot specify load address and relocation file both\n";
    }

    open(REL, '<:raw', $relocfile) or die "$0: $file: $!\n";
    read(REL, $relocs, 2*65536);
    close(REL);
} elsif (defined($org) && $abc800) {
    die "$0: abc800 only supported as relocatable\n";
}

if ($abc800) {
    print makebac800($dd, $entry, $relocs, $reserve);
} else {
    print makebac80($dd, $org, $entry, $relocs);
}
