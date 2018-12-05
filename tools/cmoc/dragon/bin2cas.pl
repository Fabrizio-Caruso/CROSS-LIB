#!/usr/bin/perl -wT

# bin2cas.pl - convert binary data to CAS or WAV with optional autorun

# Copyright (C) 2017 Ciaran Anscomb
# License: GNU GPL version 3 or later <http://www.gnu.org/licenses/gpl-3.0.html>.
# This is free software: you are free to change and redistribute it.
# There is NO WARRANTY, to the extent permitted by law.
#
# Special exception: Code included in output (such as "dunzip") is not
# considered to be part of this program, so the GPL doesn't apply to the
# results.

# See the output of "bin2cas.pl --help" for detailed usage information, with
# examples.

# Pre-v2.0: really rubbish
# Pre-v3.0: getting better, but still limited
# v3.0: complete overhaul
#       supports multiple parts
#       builds reasonably arbitrary autorun loader
#       update VDG & SAM configs during loading
#       manual cursor flasher for when video offset changed
# v3.0a: tweak default samplerate to 12kHz if fast mode used
#        slightly increase speed in fast mode
# v3.1: default timing uses varying pulse widths to account for ROM delays
#       --timing option selects between "rom" and "simple" timing
#       fast cycle timing better at 9600Hz, so dropped default back
#       slightly faster WAV creation by caching runs of samples
# v3.2: don't redirect STDOUT
#       new option --lds inserts stack adjustment code into autorun
#       license foo, --version
# v3.3: actually drop default to 9600Hz
#       dzip using temp files under windows
# v3.4: add --no-delay, --block-size, --gapped, --pause, --wav-bits
#       --cas, --wav deprecated, use --mode or rely on file extension
#       fix --no-eof
#       tweak waveform output
# v3.5: add --poke
# v3.6: use Getopt::Long and Pod::Usage
#       remove deprecated --dunzip
#       accept $ as well as 0x to indicate hex argument
# v3.7: add --vxor
# v3.8: dzipped data needs *3* bytes clear of overlap!
# v3.9: actually, dzip offset depends on number of runs at end
# v3.10: include load_part, dunzip pointers at known address
# v3.11: add --omit

use strict;

require v5.10;

use Getopt::Long;
use Pod::Usage;
use File::Temp qw/tempfile tempdir/;
use Math::Trig;
require bytes;

use constant VERSION => "3.11";

use constant {
	TYPE_BASIC => 0,
	TYPE_DATA => 1,
	TYPE_BINARY => 2,
};

use constant {
	DDOS_TYPE_BASIC => 1,
	DDOS_TYPE_BINARY => 2,
};

use constant {
	ENCODING_BINARY => 0,
	ENCODING_ASCII => 0xff,
};

use constant {
	GAP_FALSE => 0,
	GAP_TRUE => 0xff,
};

use constant {
	BLOCK_NAMEFILE => 0,
	BLOCK_DATA => 1,
	BLOCK_EOF => 0xff,
};

use constant {
	INPUT_RAW => 0,
	INPUT_DRAGONDOS => 1,
	INPUT_COCO => 2,
};

use constant DEFAULT_SAMPLE_RATE => 9600;

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

# Sekr1t options support writing specific games as accurately as possible:

# Tube Way Army:
# Loader: --name "TWA" --load 0 --exec 0 --gapped --timing rom
# Delay: --pause --pause
# Game: --no-filename --block-size 128 --block-fmt twa --timing twa

# Superkid:
# Loader: --name "SUPERKID" --load 0x012c --exec 0x4000 --timing rom
# Block0: --no-filename --no-eof --timing sk.b0
# Screen0: --no-filename --no-delay --no-eof --timing sk.s0
#          --block-fmt bare --block-size 0
# Screen1: --no-filename --no-delay --no-eof --timing sk.s1
#          --block-fmt bare --block-size 0
# Game: --no-filename --no-delay --no-eof --timing sk.g
#       --block-fmt bare --block-size 0

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

# Map DragonDOS filetypes to tape filetypes.

my %ddos_to_tape_type = (
		DDOS_TYPE_BASIC => TYPE_BASIC,
		DDOS_TYPE_BINARY => TYPE_BINARY,
		);

# Wave data.

my @wav_header = (
		0x52, 0x49, 0x46, 0x46, 0x00, 0x00, 0x00, 0x00,
		0x57, 0x41, 0x56, 0x45, 0x66, 0x6d, 0x74, 0x20,
		0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x08, 0x00, 0x64, 0x61, 0x74, 0x61,
		0x00, 0x00, 0x00, 0x00
		);

# Timings

# 'cycles' is the nominal number of CPU cycles (SAM / 16) in the waveform for
# each bit (0, 1), the rest are pulse specs.

# Pulse spec is three pairs of pulse delays (first pulse, second pulse):
# - first bit of first byte
# - remaining bits in each byte
# - first bit of subsequent bytes

# 'leader' is pulse spec while writing leader bytes and the sync byte
# 'first' is pulse spec for the first byte in a block
# 'rest' is pulse spec for remaining bytes in a block (until next leader)

# The "rom" spec is intended to be the "best fit" for the Dragon ROM -
# that is, the ROM will count an "ideal" pulse width for each bit. This should
# improve reliability.

my %timing_simple = (
		'cycles' => [ 813, 435 ],
		'leader0' => [ 0, 0, 0, 0 ],
		'leader' => [ 0, 0, 0, 0 ],
		'type' => [ 46, 0, 0, 0 ],
		'size' => [ 46, 0, 0, 0 ],
		'data0' => [ 46, 0, 0, 0 ],
		'data' => [ 46, 0, 0, 0 ],		# 818.75, 440.75 -> 629.75
		'sum' => [ 46, 0, 0, 0 ],
		);

my %timing_rom = (
		'cycles' => [ 26*28, 13*28 ],
		'leader0' => [ 205, 5, 68, 5 ],
		'leader' => [ 68, 5, 68, 5 ],
		'type' => [ 81, 5, 67, 5 ],
		'size' => [ 120, 5, 67, 5 ],
		'data0' => [ 120, 5, 67, 5 ],
		'data' => [ 120, 5, 67, 5 ],		# 806.625, 442.625 -> 625.625
		'sum' => [ 120, 5, 67, 5 ],
		);

my %timing_fast = (
		'cycles' => [ 18*28, 7*28 ],
		'leader0' => [ 205, 5, 68, 5 ],
		'leader' => [ 68, 5, 68, 5 ],
		'type' => [ 81, 5, 67, 5 ],
		'size' => [ 120, 5, 67, 5 ],
		'data0' => [ 120, 5, 67, 5 ],
		'data' => [ 120, 5, 67, 5 ],		# 582.625, 274.625 -> 428.125
		'sum' => [ 120, 5, 67, 5 ],
		);

my %timing_twa = (
		'cycles' => [ 26*28, 13*28 ],
		'leader0' => [ 114, 5, 54, 5 ],
		'leader' => [ 54, 5, 54, 5 ],
		'size' => [ 67, 5, 58, 5 ],
		'type' => [ 82, 5, 58, 5 ],
		'data0' => [ 101, 5, 58, 5 ],
		'data' => [ 106, 5, 58, 5 ],		# 797.0, 433.0 -> 615.0
		'sum' => [ 106, 5, 58, 5 ],
		);

my %timing_sk_b0 = (
		'cycles' => [ 25*28, 11*28 ],
		'leader0' => [ 205, 5, 68, 5 ],
		'leader' => [ 68, 5, 68, 5 ],
		'type' => [ 81, 5, 67, 5 ],
		'size' => [ 120, 5, 67, 5 ],
		'data0' => [ 120, 5, 67, 5 ],
		'data' => [ 120, 5, 67, 5 ],
		'sum' => [ 120, 5, 67, 5 ],
		);

my %timing_sk_s0 = (
		'cycles' => [ 25*28, 11*28 ],
		'data0' => [ 89, 5, 67, 5 ],
		'data' => [ 112, 5, 67, 5 ],
		);

my %timing_sk_s1 = (
		'cycles' => [ 25*28, 11*28 ],
		'data0' => [ 115, 5, 67, 5 ],
		'data' => [ 112, 5, 67, 5 ],
		);

my %timing_sk_g = (
		'cycles' => [ 25*28, 11*28 ],
		'leader' => [ 132, 5, 67, 5 ],
		'data0' => [ 125, 5, 67, 5 ],
		'data' => [ 132, 5, 67, 5 ],
		'adj_period' => 8,
		'adj' => 6,
		);

my %timing_by_name = (
		'simple' => \%timing_simple,
		'rom' => \%timing_rom,
		'fast' => \%timing_fast,
		'twa' => \%timing_twa,
		'sk.b0' => \%timing_sk_b0,
		'sk.s0' => \%timing_sk_s0,
		'sk.s1' => \%timing_sk_s1,
		'sk.g' => \%timing_sk_g,
		);

# Block format strings

my %fmt_strings = (
	'bare' => 'd',
	'twa' => 'iSTdCo',
	'normal' => 'itsdco',
);

# Autorun headers can include optional parts, concatenated and subject to
# linking.

my @code_load_0 = (
	"load_part",
		0x9f, 0x7e,		# stx	<$7e
		0xad, 0x9f, 0xa0, 0x04,	# jsr	[CSRDON]
	"l0",
);

my @code_load_flash = (
		0xb6, ">flash_addr",	# lda	>flash_addr
		0x88, 0x40,		# eora	#$40
	# flash code starts off disabled (first load will still be in text mode)
	"mod_flash",
		0x8c, ">flash_addr",	# cmpx	>flash_addr
);

my @code_load_1 = (
		0xad, 0x9f, 0xa0, 0x06,	# jsr	[BLKIN]
		0x26, "\&<do_io_error",	# bne	do_io_error
		0x9f, 0x7e,		# stx	<$7e
		0x96, 0x7c,		# lda	<$7c
		0x4c,			# inca
		0x26, "\&<l0",		# bne	l0
	"cas_off",
		0xb6, 0xff, 0x21,	# lda	>$ff21
		0x84, 0xf7,		# anda	#$f7
		0xb7, 0xff, 0x21,	# sta	>$ff21
	"do_rts",
		0x39,			# rts
	"do_io_error",
		0x8d, "\&<cas_off",	# bsr	cas_off
		0x8e, ">io_error",	# ldx	#io_error
		0x8d, "\&<out_string",	# bsr	out_string
	"l1",
		0x20, "\&<l1",		# bra	l1
	"out_string",
		0xa6, 0x80,		# lda	,x+
		0x27, "\&<do_rts",	# beq	do_rts
		0xad, 0x9f, 0xa0, 0x02,	# jsr	[OUTCH]
		0x20, "\&<out_string",	# bra	out_string
	"io_error",
		0x49, 0x2f, 0x4f, 0x20,	# fcc	"I/O "
		0x45, 0x52, 0x52, 0x4f,	# fcc	"ERRO"
		0x52, 0x00,		# fcc	"R",0
);

my @code_disable_flasher = (
		0x86, 0x8c,		# lda	#$8c		; cmpx
		0xb7, ">mod_flash",	# sta	mod_flash
);

my @code_enable_flasher = (
		0x86, 0xb7,		# lda	#$8c		; sta
		0xb7, ">mod_flash",	# sta	mod_flash
);

my @code_test_arch = (
		0xb6, 0xa0, 0x00,	# lda	$a000
		0x84, 0x20,		# anda	#$20
		0x97, 0x10,		# sta	<$10
);

my @code_fast = (
		0xcc, ">fast_pw",	# ldd	#fast_pw
		0x0d, 0x10,		# tst	<$10
		0x26, "\&<fl1",		# bne	fl1
		0xdd, 0x92,		# std	<$92
		0x97, 0x94,		# sta	<$94
		0x20, "\&<fl2",		# bra	fl2
	"fl1",
		0xdd, 0x90,		# std	<$90
		0x97, 0x8f,		# sta	<$8f
	"fl2",
);

my @code_dunzip = (
	"dunzip",
		0x34, 0x06,		# pshs	d
	"dunz_loop",
		0xec, 0x81,		# ldd	,x++
		0x2a, "\&<dunz_run",	# bpl	dunz_run
		0x5d,			# tstb
		0x2a, "\&<dunz_7_7",	# bpl	dunz_7_7
	"dunz_14_8",
		0x58,			# lslb
		0x47,			# asra
		0x56,			# rorb
		0x31, 0xcb,		# leay	d,u
		0xe6, 0x80,		# ldb	,x+
		0x20, "\&<dl0",		# bra	dl0
	"dunz_7_7",
		0x31, 0xc6,		# leay	a,u
	"dl0",
		0xa6, 0xa0,		# lda	,y+
		0xa7, 0xc0,		# sta	,u+
		0x5c,			# incb
		0x28, "\&<dl0",		# bvc	dl0
		0x20, "\&<dl2",		# bra	dl2
	"dl1",
		0xe6, 0x80,		# ldb	,x+
	"dunz_run",
		0xe7, 0xc0,		# stb	,u+
		0x4c,			# inca
		0x28, "\&<dl1",		# bvc	dl1
	"dl2",
		0xac, 0xe4,		# cmpx	,s
		0x25, "\&<dunz_loop",	# blo	dunz_loop
		0x35, 0x86,		# puls	d,pc
);

# Reverse dunzip.  Ok to reuse labels as only one will be included.

my @code_dunzipr = (
	"dunzip",
		0x34, 0x06,		# pshs	d
	"dunz_loop",
		0xec, 0x83,		# ldd	,--x
		0x2b, "\&<dunz_run",	# bmi	dunz_run
		0x5d,			# tstb
		0x2a, "\&<dunz_7_7",	# bpl	dunz_7_7
	"dunz_14_8",
		0x58,			# lslb
		0x44,			# lsra
		0x56,			# rorb
		0x31, 0xcb,		# leay	d,u
		0xe6, 0x82,		# ldb	,-x
		0x20, "\&<dj0",		# bra	dj0
	"dunz_7_7",
		0x31, 0xc6,		# leay	a,u
	"dj0",
		0x31, 0x21,		# leay	1,y
	"dl0",
		0xa6, 0xa2,		# lda	,-y
		0xa7, 0xc2,		# sta	,-u
		0x5c,			# incb
		0x28, "\&<dl0",		# bvc	dl0
		0x20, "\&<dl2",		# bra	dl2
	"dl1",
		0xe6, 0x82,		# ldb	,-x
	"dunz_run",
		0xe7, 0xc2,		# stb	,-u
		0x4c,			# inca
		0x26, "\&<dl1",		# bne	dl1
	"dl2",
		0xac, 0xe4,		# cmpx	,s
		0x22, "\&<dunz_loop",	# bhi	dunz_loop
		0x35, 0x86,		# puls	d,pc
);

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

# Output options

my $out_filename;
my $wav_out;
my $sample_rate;
my $bits_per_sample = 8;

# Global options

my $autorun = 0;
my $leader = 256;

# Per-file options

my $input_mode = INPUT_RAW;
my $want_name;
my $want_load;
my $want_exec;
my $want_zload;
my $want_fnblock;

my $want_delay = 1;
my $block_size = 255;
my $block_fmt = 'normal';
my $want_dzip = 0;
my $want_vxor = 0;
my $want_eof = 1;
my $want_eof_data = 0;
my $want_gapped = 0;
my $want_timing;
my $want_omit = 0;

my $want_flasher = 0;

# Persistent

my $autorun_name;
my $autorun_exec;
my $any_dzip = 0;
my $any_fast = 0;
my $any_flasher = 0;
my $reverse_dzip = 0;

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

my $out_fd;

# Relocation

my $mc_org;
my $mc_pc;
my %mc_label = ();
my @mc_link;
my $mc_data;

# WAV

my $num_channels = 1;
my $sample_count = 0;
my $ao_error = 0;
my %sinewaves = ();
my $normal_timing = 'rom';
my $timing;

my $adj_period = 0;
my $adj = 0;
my $adj_count = 0;

# this combination of slow cycle pulse width boundaries seems to allow reliable
# tape speed variance of at least +/-7% when emitting 30% faster data to a
# 9600Hz WAV
$mc_label{'fast_pw'} = 0x0c06;

# de-taint PATH for call out to dzip
if ($ENV{'PATH'} =~ /^(.*)$/) {
	$ENV{'PATH'} = $1;
}

my @files = ();
my %file = ();

Getopt::Long::Configure("gnu_getopt", "pass_through");

# helper to parse integer options - allow hex values prefixed by $ or 0x
sub eval_int {
	my $string = shift;
	$string =~ s/^\$/0x/;
	if ($string =~ /^(\d+|0x[\da-f]+)$/i) {
		return eval $1;
	}
	die "invalid number format: $string\n";
}

GetOptions(
		# Output options:
		"output|o=s" => \$out_filename,
		"mode|m=s" => sub {
			shift;
			my $v = lc(shift @ARGV);
			$wav_out = ($v eq 'wav');
		},
		"cas" => sub { $wav_out = 0; },
		"wav-out|wav" => sub { $wav_out = 1; },
		"wav-rate|r=s" => sub { shift; $sample_rate = eval_int(shift) },
		"wav-bits=s" => sub { shift; $bits_per_sample = eval_int(shift) },

		# Global options:
		"autorun|A!" => \$autorun,
		"leader=s" => sub { shift; $leader = eval_int(shift) },

		# Per-file options:
		"B" => sub { $input_mode = INPUT_RAW; },
		"D" => sub { $input_mode = INPUT_DRAGONDOS; },
		"C" => sub { $input_mode = INPUT_COCO; },

		"name|n=s" => sub {
			shift;
			$want_name = shift;
			$autorun_name = $want_name;
		},
		"load|l=s" => sub { shift; $want_load = eval_int(shift) },
		"exec|e=s" => sub {
			shift;
			$want_exec = eval_int(shift);
			$autorun_exec = $want_exec;
		},
		"zload=s" => sub { shift; $want_zload = eval_int(shift) },
		"filename!" => \$want_fnblock,

		"delay!" => \$want_delay,
		"block-size=s" => sub { shift; $block_size = eval_int(shift) },
		"block-fmt=s" => \$block_fmt,
		"eof!" => \$want_eof,
		"gapped!" => \$want_gapped,
		"timing|T=s" => sub {
			shift;
			$want_timing = shift;
			$any_fast = 1 if ($want_timing eq 'fast');
		},
		"fast" => sub {
			$want_timing = 'fast';
			$any_fast = 1;
		},
		"no-fast" => sub { undef $want_timing; },
		"omit!" => \$want_omit,

		"no-flasher|noflasher" => sub { $want_flasher = 0; },
		"flasher" => sub {
			$want_flasher = 1;
			$any_flasher = 1;
		},
		"vxor|x=s" => sub {
			shift;
			$want_vxor = shift;
		},
		"no-vxor|novxor" => sub { $want_vxor = 0; },
		"i=s" => sub { shift; push @files, input_file(shift); },

		# Per-file persistent options:
		"dzip|z" => sub {
			$want_dzip = 1;
			$any_dzip = 1;
		},
		"no-dzip|nodzip" => sub { $want_dzip = 0; },
		"reverse-dzip" => sub { $reverse_dzip = 1; },
		"eof-data|E!" => \$want_eof_data,

		# Inter-file options:
		"lds=s" => sub {
			shift;
			push @files, { 'code' => "lds", 'value' => eval_int(shift) };
		},
		"pause" => sub {
			push @files, { 'code' => "pause", 'value' => 0 };
		},
		"sam-f=s" => sub {
			shift;
			push @files, { 'code' => "set_sam_f", 'value' => eval_int(shift) };
		},
		"sam-v=s" => sub {
			shift;
			my $v = eval_int(shift);
			push @files, { 'code' => "set_sam_v", 'value' => $v };
		},
		"vdg=s" => sub {
			shift;
			push @files, { 'code' => "set_vdg", 'value' => eval_int(shift) };
		},
		"poke=s" => sub {
			shift;
			my $v = shift;
			push @files, { 'code' => "poke", 'value' => $v };
		},

		# Other options:
		"version" => \&version_text,
		"help|?" => sub { pod2usage(-verbose => 2); },

		# pass_through option means anything else hits this, so die if
		# it looks like an option we don't handle, else treat it as an
		# input file:
		"<>" => sub {
			my $file = shift;
			die "Unrecognised option: $file\n" if ($file =~ /^-/);
			push @files, input_file($file);
		},
	  ) or exit(2);

die "invalid value for --wav-bits\n" if ($bits_per_sample != 8 && $bits_per_sample != 16);

if (defined $out_filename && $out_filename =~ /(.*)/) {
	$out_filename = $1;  # de-taint
} else {
	die "No output filename specified\n";
}

if (@ARGV) {
	shift @ARGV;  # discard --
	while (@ARGV) {
		push @files, input_file(shift @ARGV);
	}
}

# Prepare output stream.

open($out_fd, ">", $out_filename) or die $!;
binmode $out_fd;
if ($out_filename =~ /\.cas$/i) {
	$wav_out //= 0;
} elsif ($out_filename =~ /\.wav$/i) {
	$wav_out //= 1;
}

$sample_rate //= DEFAULT_SAMPLE_RATE;
my $sam_rate = 14318180;
my $bytes_per_sample = $bits_per_sample >> 3;

# WAV header?
if ($wav_out) {
	# NumChannels
	$wav_header[22] = $num_channels;
	$wav_header[23] = ($num_channels >> 8) & 0xff;
	# SampleRate
	$wav_header[24] = $sample_rate & 0xff;
	$wav_header[25] = ($sample_rate >> 8) & 0xff;
	$wav_header[26] = ($sample_rate >> 16) & 0xff;
	$wav_header[27] = ($sample_rate >> 24) & 0xff;
	# ByteRate
	my $byte_rate = $sample_rate * $num_channels * $bytes_per_sample;
	$wav_header[28] = $byte_rate & 0xff;
	$wav_header[29] = ($byte_rate >> 8) & 0xff;
	$wav_header[30] = ($byte_rate >> 16) & 0xff;
	$wav_header[31] = ($byte_rate >> 24) & 0xff;
	# BlockAlign
	my $block_align = ($num_channels * $bits_per_sample) / 8;
	$wav_header[32] = $block_align & 0xff;
	$wav_header[33] = ($block_align >> 8) & 0xff;
	# BitsPerSample
	$wav_header[34] = $bits_per_sample & 0xff;
	$wav_header[35] = ($bits_per_sample >> 8) & 0xff;
	print $out_fd pack("C*", @wav_header);
}

# Write file(s).

write_autorun(\@files) if ($autorun);
write_all_files(\@files);

# Close output.

if ($wav_out) {
	# rewrite Subchunk2Size
	$sample_count *= $bytes_per_sample;
	seek($out_fd, 40, 0);
	print $out_fd pack("C", $sample_count & 0xff);
	print $out_fd pack("C", ($sample_count >> 8) & 0xff);
	print $out_fd pack("C", ($sample_count >> 16) & 0xff);
	print $out_fd pack("C", ($sample_count >> 24) & 0xff);
	# rewrite ChunkSize
	$sample_count += 36;
	seek($out_fd, 4, 0);
	print $out_fd pack("C", $sample_count & 0xff);
	print $out_fd pack("C", ($sample_count >> 8) & 0xff);
	print $out_fd pack("C", ($sample_count >> 16) & 0xff);
	print $out_fd pack("C", ($sample_count >> 24) & 0xff);
}

exit 0;

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

sub reset_default_options {
	$input_mode = INPUT_RAW;
	undef $want_name;
	undef $want_load;
	undef $want_exec;
	undef $want_zload;
	undef $want_fnblock;
	$want_delay = 1;
	$block_size = 255;
	$block_fmt = 'normal';
	$want_eof = 1;
	$want_gapped = 0;
	undef $want_timing;
	$want_flasher = 0;
	$want_vxor = 0;
}

# Read a file, applying current default options, reset default options.

sub input_file {
	my ($filename,$file) = @_;
	$file //= {};

	# defaults
	$file->{'fnblock'} //= $want_fnblock;
	$file->{'delay'} //= $want_delay;
	$file->{'block_size'} //= $block_size;
	$file->{'block_fmt'} //= $block_fmt;
	$file->{'eof'} //= $want_eof;
	$file->{'eof_data'} //= $want_eof_data;
	$file->{'gapped'} //= $want_gapped;
	$file->{'timing'} //= $want_timing;
	$file->{'flasher'} //= $want_flasher;
	$file->{'omit'} //= $want_omit;

	# read in appropriate mode
	$file = do {
		if ($input_mode == INPUT_DRAGONDOS) {
			read_dragondos($filename, $file);
		} elsif ($input_mode == INPUT_COCO) {
			read_coco($filename, $file);
		} else {
			read_raw($filename, $file);
		}
	};

	# overrides
	$file->{'name'} = $want_name if (defined $want_name);
	$file->{'load'} = $want_load if (defined $want_load);
	$file->{'exec'} = $want_exec if (defined $want_exec);
	$file->{'zload'} = $want_zload if (defined $want_zload);

	die "No data\n" unless exists $file->{'segments'};

	# XXX only deal with single-segment binaries for now
	coalesce_file($file);

	vxor_file($file) if ($want_vxor);
	dzip_file($file) if ($want_dzip);

	reset_default_options();

	return $file;
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

# File readers

sub open_file {
	my ($filename,$file) = @_;
	$file //= {};
	open(my $in, "<", $filename) or die "Failed to open $filename: $!\n";
	binmode $in;
	if ($filename =~ /^([^\.]{1,8})/) {
		$file->{'name'} //= uc $1;
	}
	return $in;
}

# Raw binary: just slurp data into one segment
sub read_raw {
	my ($filename,$file) = @_;
	$file //= {};
	my $in = open_file($filename, $file);
	my %segment = (
		'start' => 0,
		'size' => 0,
		'data' => "",
	);
	my $data;
	my $rsize;
	do {
		$rsize = read $in, $data, 0x10000;
		$segment{'data'} .= $data;
		$segment{'size'} += $rsize;
	} while ($rsize == 0x10000);
	$file->{'segments'} = [ \%segment ];
	close $in;
	return $file;
}

# DragonDOS binary - single segment only
sub read_dragondos {
	my ($filename,$file) = @_;
	my $in = open_file($filename, $file);

	getc($in);  # skip $55
	my $type = unpack("C", getc($in));
	my $start = (unpack("C", getc($in)) << 8) | unpack("C", getc($in));
	my $size = (unpack("C", getc($in)) << 8) | unpack("C", getc($in));
	my $exec = (unpack("C", getc($in)) << 8) | unpack("C", getc($in));
	getc($in);  # skip $aa

	my $data;
	my $rsize = read $in, $data, $size;
	if ($rsize != $size) {
		print STDERR "Warning: short read from DragonDOS binary\n";
	}
	my %segment = (
		'start' => $start,
		'data' => $data,
		'size' => $rsize,
	);
	$file->{'segments'} = [ \%segment ];
	close $in;

	$file->{'type'} //= $ddos_to_tape_type{$type} // TYPE_BINARY;
	$file->{'exec'} //= $exec;
	return $file;
}

# CoCo (DECB) - binaries can contain multiple segments

# BASIC files are: $ff size>>8 size data*
# BINARY files are: ($00 size>>8 size data*)+ $ff 00 00 exec>>8 exec
#   (binaries can contain multiple segments)

sub read_coco {
	my ($filename,$file) = @_;
	my $in = open_file($filename, $file);

	my $type;
	my $exec;

	while (my $stype = getc($in)) {
		$stype = unpack("C", $stype);

		my $start;
		my $size = (unpack("C", getc($in)) << 8) | unpack("C", getc($in));

		if ($stype == 0x00) {
			$type //= TYPE_BINARY;
			$start = (unpack("C", getc($in)) << 8) | unpack("C", getc($in));
		} elsif (!defined $type && $stype == 0xff) {
			$type = TYPE_BASIC;
			$start = 0;
			$exec = 0;
		} elsif ($stype == 0xff) {
			if ($size != 0) {
				# XXX is this dodgy?
				printf STDERR "Warning: EXEC segment with non-zero size in CoCo binary\n";
			}
			$exec = (unpack("C", getc($in)) << 8) | unpack("C", getc($in));
		} else {
			printf STDERR "Warning: skipping data in CoCo binary\n";
			last;
		}

		if ($size > 0) {
			my $data;
			my $rsize = read $in, $data, $size;
			if ($rsize != $size) {
				print STDERR "Warning: short read from CoCo binary\n";
			}
			push @{$file->{'segments'}}, {
				'start' => $start,
				'data' => $data,
				'size' => $rsize,
			};
		}
	}
	close $in;

	$file->{'type'} //= $type // TYPE_BINARY;
	$file->{'exec'} //= $exec if (defined $exec);
	return $file;
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

# Replace zero or more segments with exactly one zero-padded segment. All
# other file information preserved.

sub coalesce_file {
	my $file = shift;
	return if (exists $file->{'segments'} && scalar(@{$file->{'segments'}}) == 1);
	my $old_segments = $file->{'segments'} // [];
	my %segment = ();
	my $end;
	for my $s (sort { $a->{'start'} <=> $b->{'start'} } @{$old_segments}) {
		my $start = $s->{'start'};
		$segment{'start'} //= $start;
		$end //= $start;
		my $size = bytes::length($s->{'data'});
		my $new_end = $start + $size;
		# TODO
		if ($start < $end) {
			die "Can't handle overlapping segments\n";
		}
		# TODO
		if ($end >= 0x10000) {
			die "Segment too large\n";
		}
		if ($start > $end) {
			$segment{'data'} .= "\0" x ($start - $end);
		}
		$segment{'data'} .= $s->{'data'};
		$end = $new_end;
	}
	$segment{'data'} //= "";
	$segment{'start'} //= 0;
	$end //= $segment{'start'};
	$segment{'size'} = $end - $segment{'start'};
	$file->{'segments'} = [ \%segment ];
}

# Pass file data to dzip. Replaces data, preserves original file metadata and
# "osize" records original data size.

sub dzip_file {
	my $file = shift;
	coalesce_file($file);  # single-segment only
	my $segment = $file->{'segments'}[0];
	my $osize = $segment->{'size'} // bytes::length($segment->{'data'});
	my $zdata = "";
	my $cfd;
	my $tmp_filename;
	if ($^O eq 'MSWin32') {
		# So windows doesn't support forked pipes or list open(). Ack!
		# Ok, write binary to a temporary file, dzip it, then read in
		# the results. This is dumb.
		my $tmp_dir = tempdir(CLEANUP => 1);  # auto-clean on exit
		my $tmp_fd;
		($tmp_fd, $tmp_filename) = tempfile(DIR => $tmp_dir);
		binmode $tmp_fd;
		print $tmp_fd $segment->{'data'};
		close $tmp_fd;
		my $opts = $reverse_dzip ? "-rk" : "-k";
		system("dzip", $opts, $tmp_filename);
		open($cfd, "<", "$tmp_filename.dz") or die "Can't open dzipped tempfile\n";
	} else {
		# For everything else, just pass the binary out to a fork
		# running dzip. No race.
		my $pid = open($cfd, "-|") // die "Failed to open pipe to dzip\n";
		if ($pid == 0) {
			my $opts = $reverse_dzip ? "-rc" : "-c";
			open(my $zfd, "|-", "dzip", $opts) // exit 0;
			binmode $zfd;
			print $zfd $segment->{'data'};
			close $zfd;
			exit 0;
		}
	}
	binmode $cfd;
	{
		local $/ = undef;
		$zdata = <$cfd>;
	}
	close $cfd;
	if ($^O eq 'MSWin32') {
		unlink "$tmp_filename.dz";
	}
	# reposition...
	my $zsize = bytes::length($zdata);
	die "No data from pipe to dzip\n" unless ($zsize > 0);
	$segment->{'dzip'} = 1;
	$segment->{'data'} = $zdata;
	$segment->{'size'} = $zsize;
	$segment->{'osize'} = $osize;
	if ($reverse_dzip) {
		$segment->{'zoffset'} = rdzip_end_runcount(\$zdata);
	} else {
		$segment->{'zoffset'} = dzip_end_runcount(\$zdata);
	}
}

# Skim through zipped data.  The number of consecutive runs at the end of the
# zipped data is the number of bytes required to not overlap.

sub dzip_end_runcount {
	my $zdata = shift;
	my @data = unpack("C*", $$zdata);
	my $length = scalar(@data);
	my $runcount = 0;
	my $max_runcount = 0;
	my $i = 0;
	while ($i < $length) {
		my $a = $data[$i++];
		if ($a & 0x80) {
			$max_runcount = $runcount if ($runcount > $max_runcount);
			$runcount = 0;
			if ($data[$i] & 0x80) {
				$i++;
			}
			$i++;
		} else {
			$runcount++;
			$i += (128 - $a);
		}
	}
	return $max_runcount;
}

sub rdzip_end_runcount {
	my $zdata = shift;
	my @data = unpack("C*", $$zdata);
	my $length = scalar(@data);
	my $runcount = 0;
	my $i = $length;
	while ($i > 0) {
		$i -= 2;
		my $a = $data[$i];
		if ($a & 0x80) {
			$runcount = 0;
			if ($data[$i] & 0x80) {
				$i--;
			}
		} else {
			$runcount++;
			$i -= (127 - $a);
		}
	}
	return $runcount;
}

sub vxor_file {
	my $file = shift;
	coalesce_file($file);  # single-segment only
	my $segment = $file->{'segments'}[0];
	my $size = $segment->{'size'} // bytes::length($segment->{'data'});
	my @vdata = ();
	my $stride = $want_vxor;
	my @vxor = (0) x $stride;
	for my $i (0..$size-1) {
		my $c = unpack("C", substr($segment->{'data'}, $i, 1));
		$vdata[$i] = $c ^ $vxor[$i % $stride];
		$vxor[$i % $stride] = $c;
	}
	$segment->{'vxor'} = 1;
	$segment->{'stride'} = $stride;
	$segment->{'data'} = pack("C*", @vdata);
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

sub set_timing {
	my $name = shift;
	$name //= $normal_timing;
	if ($name eq 'simple' || $name eq 'rom') {
		$normal_timing = $name;
	}
	die "invalid timing name '$name'\n" unless exists $timing_by_name{$name};
	$timing = $timing_by_name{$name};
}

# Write a file.

sub write_file {
	my $file = shift;

	# defaults
	$file->{'fnblock'} //= 1;
	$file->{'eof'} //= 1;
	set_timing($file->{'timing'});

	# XXX only deal with single-segment files
	coalesce_file();
	my $segment0 = $file->{'segments'}[0];

	# Write filename block if required.
	if ($file->{'fnblock'}) {
		my $name = $file->{'name'} // "BINARY";
		my $type = $file->{'type'} // TYPE_BINARY;
		my $encoding = $file->{'encoding'} // ENCODING_BINARY;
		my $gap = $file->{'gapped'} ? GAP_TRUE : GAP_FALSE;
		my $load = $file->{'load'} // $segment0->{'start'} // 0x0e00;
		my $exec = $file->{'exec'} // $load;
		my $fndata = sprintf("\%-8s", $name);
		$fndata .= pack("CCCnn", $type, $encoding, $gap, $exec, $load);
		write_leader();
		block_out(BLOCK_NAMEFILE, $fndata, $file->{'block_fmt'});
	}

	# Write file data.
	write_leader() if (!$file->{'gapped'} && $file->{'delay'});
	my $data = $segment0->{'data'};
	my $size = $segment0->{'size'};
	my $ptr = 0;
	while ($size > 0) {
		write_leader() if ($file->{'gapped'});
		my $bsize;
		if ($file->{'block_size'} > 0 && $size > $file->{'block_size'}) {
			$bsize = $file->{'block_size'};
		} else {
			$bsize = $size;
		}
		$size -= $bsize;
		if ($size == 0 && $file->{'eof'}) {
			if ($file->{'eof_data'}) {
				block_out(BLOCK_EOF, bytes::substr($data, $ptr, $bsize), $file->{'block_fmt'});
			} else {
				block_out(BLOCK_DATA, bytes::substr($data, $ptr, $bsize), $file->{'block_fmt'});
				write_leader() if ($file->{'gapped'});
				block_out(BLOCK_EOF, "", $file->{'block_fmt'});
			}
		} else {
			block_out(BLOCK_DATA, bytes::substr($data, $ptr, $bsize), $file->{'block_fmt'});
		}
		$ptr += $bsize;
	}
}

# Write an autorun file, using data from a list of files.

sub write_autorun {
	my $files = shift;

	# Ensure autorun program is stored at normal speed.
	set_timing();

	my $vdg = 0;
	my $sam_v = 0;
	my $sam_f = 2;  # 0x0400
	my $flasher = 0;
	$mc_label{'flash_addr'} = 0x0400;

	my $name = $autorun_name // "AUTORUN";

	# Construct a special filename block to autorun. This builds the data
	# out of blocks of code and then "links" it, as the contents can vary
	# (multiple files to load, fast loading, dunzipping data).

	# The standard part of the filename block.
	mcdata_org(0x01da);
	mcdata_add(\sprintf("\%-8s", $name));	# filename
	mcdata_add([
			TYPE_BINARY,		# file type: machine code
			ENCODING_BINARY,	# ascii flag: binary
			GAP_FALSE,		# gap flag: continuous
		"colon",
			0x3a, 0x00,		# exec address (convenient ':')
			0x00, 0xa6,		# load address: BASIC next
		"load_part_ptr", ">load_part",
		]);
	if ($any_dzip) {
		mcdata_add([
		"dunzip_ptr", ">dunzip",
			]);
	}
	mcdata_add([
		"exec_loader",			# main code starts at $01eb or $01ed
	]);

	# Include fast timing setup if necessary.
	if ($any_fast && $wav_out) {
		mcdata_add(\@code_test_arch);
		mcdata_add(\@code_fast);
	}

	# For each file, add loading instructions.
	my $last_exec = 0;
	for my $file (@{$files}) {
		# defaults when autorunning
		$file->{'fnblock'} //= 0;
		$file->{'eof_data'} //= 1;
		# TODO: currently no provision for switching speed mid-load:
		$file->{'timing'} = 'fast' if ($any_fast);

		# a "file" might just be some specific code instructions
		my $code = $file->{'code'} // "";
		my $value = $file->{'value'} // 0;
		if ($code eq "set_vdg") {
			$value &= 0xf8;
			if ($vdg != $value) {
				mcdata_add([
						0x86, $file->{'value'},	# lda	#value
						0xb7, 0xff, 0x22,	# sta	>$ff22
					]);
			}
			$vdg = $value;
		} elsif ($code eq "set_sam_v") {
			for my $i (0..2) {
				my $bit = (1 << $i);
				next if ((($sam_v ^ $value) & $bit) == 0);
				my $addr = 0xc0 + $i * 2;
				$addr++ if ($value & $bit);
				mcdata_add([
						0xb7, 0xff, $addr,	# sta	>$ffxx
					]);
			}
			$sam_v = $value & 7;
		} elsif ($code eq "set_sam_f") {
			for my $i (0..6) {
				my $bit = (1 << $i);
				next if ((($sam_f ^ $value) & $bit) == 0);
				my $addr = 0xc6 + $i * 2;
				$addr++ if ($value & $bit);
				mcdata_add([
						0xb7, 0xff, $addr,	# sta	>$ffxx
					]);
			}
			$sam_f = $value & 0x7f;
			$mc_label{'flash_addr'} = $sam_f * 512;
		} elsif ($code eq "lds") {
			mcdata_add([
					0x10, 0xce, $value>>8, $value&0xff,	# lds	#value
					]);
		} elsif ($code eq "poke") {
			my ($aa,$vv) = split(/,/, $value);
			my $addr = eval_int($aa);
			my $val = eval_int($vv);
			mcdata_add([
					0x86, $val&0xff,		# lda	#value
					0xb7, $addr>>8, $addr&0xff,	# sta	>addr
					]);
		} elsif ($code eq "pause") {
			write_pause();
		} elsif ($file->{'omit'}) {
			next;
		} else {
			my $segment0 = $file->{'segments'}[0];
			my $load = $file->{'load'} // $segment0->{'start'};
			my $size = $segment0->{'size'};
			my $oload = $load;
			my $osize = $size;
			if ($segment0->{'dzip'}) {
				$osize = $segment0->{'osize'};
				my $zoffset = $segment0->{'zoffset'};
				if ($reverse_dzip) {
					$load = $file->{'zload'} // $oload - 3;
				} else {
					$load = $file->{'zload'} // $oload + $osize + $zoffset - $size;
				}
			}
			my $end = $load + $size;
			my $fflasher = $file->{'flasher'} // 0;
			if ($flasher && !$fflasher) {
				mcdata_add(\@code_disable_flasher);
			} elsif (!$flasher && $fflasher) {
				mcdata_add(\@code_enable_flasher);
			}
			$flasher = $fflasher;
			mcdata_add([
					0x8e, $load>>8, $load&0xff,	# ldx	#load
					0x8d, "\&<load_part",	# bsr	load_part
				]);
			if (!$reverse_dzip && $segment0->{'dzip'}) {
				mcdata_add([
						0x8e, $load>>8, $load&0xff,	# ldx	#load
						0xcc, $end>>8, $end&0xff,	# ldd	#end
						0xce, $oload>>8, $oload&0xff,	# ldu	#oload
						0x8d, "\&<dunzip",	# bsr	dunzip
					]);
			}
			if ($reverse_dzip && $segment0->{'dzip'}) {
				my $end = $oload + $osize;
				mcdata_add([
						0xcc, $load>>8, $load&0xff,	# ldd	#load
						0xce, $end>>8, $end&0xff,	# ldu	#end
						0x8d, "\&<dunzip",	# bsr	dunzip
					]);
			}
			if ($segment0->{'vxor'}) {
				my $stride = $segment0->{'stride'};
				my $start = $oload + $stride;
				my $end = $oload + $osize;
				mcdata_add([
						0x8e, $start>>8, $start&0xff,	# ldx #start
						0xa6, 0x88, 0x100-$stride,	# lda -stride,x
						0xa8, 0x84,		# eora ,x
						0xa7, 0x80,		# sta ,x+
						0x8c, $end>>8, $end&0xff,	# cmpx #end
						0x25, 0xf4,	# blo loop
				]);
			}

			$last_exec = $file->{'exec'};
		}
	}
	$mc_label{'exec'} = $autorun_exec // $last_exec // 0;

	# Finally, JMP to the program.
	mcdata_add([
			0x7e, ">exec",		# jmp	exec
			]);

	# Optional and non-optional chunks of code in support of above.
	mcdata_add(\@code_load_0);
	mcdata_add(\@code_load_flash) if ($any_flasher);
	mcdata_add(\@code_load_1);
	if ($any_dzip) {
		if ($reverse_dzip) {
			mcdata_add(\@code_dunzipr);
		} else {
			mcdata_add(\@code_dunzip);
		}
	}

	# Link - replace all symbol references with actual addresses.
	mcdata_link();

	# Write filename block.
	write_leader();
	block_out(BLOCK_NAMEFILE, $mc_data);

	# Now the data portion of the loader. Tiny, just overwrites "next
	# basic token with a pointer to that convenient ':' in the filename
	# block, and makes BASIC JMP to the loader code.
	mcdata_org(0x00a6);
	mcdata_add([
			">colon",		# fdb	colon
			0x7e, ">exec_loader",	# jmp	exec_loader
			]);
	mcdata_link();
	write_leader();
	block_out(BLOCK_EOF, $mc_data);
}

# Write a list of files

sub write_all_files {
	my $files = shift;
	for my $file (@{$files}) {
		my $code = (exists $file->{'code'}) ? $file->{'code'} : "";
		my $value = $file->{'value'} // 0;
		if ($code eq 'pause') {
			write_pause();
		} elsif ($code eq '') {
			write_file($file);
		}
	}
	bytes_out("U", $timing->{'leader'}) if ($leader > 0);
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

sub write_pause {
	if ($wav_out) {
		sample_out(0x80, 0xda5c * 8);
		$ao_error = 0.5;
	} else {
		bytes_out("U" x 94, $timing->{'leader'});
		# NOTE: CAS files can't be fast, but if they were, typical
		# pause would be 136 leader bytes
	}
}

sub write_leader {
	write_pause();
	bytes_out("U" x $leader, $timing->{'leader'});
}

sub block_out {
	my ($btype,$data,$fmt) = @_;
	$fmt = $fmt_strings{$fmt // 'normal'};
	my $bsize = bytes::length($data);
	my $sum = 0;
	$adj_period = $timing->{'adj_period'} // 0;
	$adj = $timing->{'adj'} // 0;
	$adj_count = 0;
	for my $c (split(//, $fmt)) {
		my $eor = 0;
		$eor = 0xff if ($c eq uc $c);
		$c = lc $c;
		if ($c eq 'i') {
			# block in: leader byte, $3c
			bytes_out(pack("C*", 0x55, 0x3c), $timing->{'leader'});
		} elsif ($c eq 'o') {
			# block out: leader byte, initial timing
			bytes_out(pack("C", 0x55), $timing->{'leader0'});
		} elsif ($c eq 't') {
			# block type
			bytes_out(pack("C", $btype^$eor), $timing->{'type'});
			$sum += $btype;
		} elsif ($c eq 's') {
			# block size
			bytes_out(pack("C", $bsize^$eor), $timing->{'size'});
			$sum += $bsize;
		} elsif ($c eq 'c') {
			# block checksum
			bytes_out(pack("C", ($sum^$eor)&0xff), $timing->{'size'});
		} elsif ($c eq 'd') {
			# block data (invert not supported yet)
			bytes_out($data, $timing->{'data0'}, $timing->{'data'});
			for (unpack("C*", $data)) {
				$sum += $_;
			}
		}
	}
}

sub ao_period {
	my $cycles = shift;
	my $period = ($sample_rate * $cycles * 16) / $sam_rate;
	my $iperiod = int($period);
	$ao_error += ($period - $iperiod);
	$iperiod += int($ao_error);
	$ao_error -= int($ao_error);
	return $iperiod;
}

sub bytes_out {
	my $data = shift;
	my $pspec = shift;
	my $delay0 = $pspec->[0];
	my $delay1 = $pspec->[1];
	if (!$wav_out) {
		print $out_fd $data;
		return;
	}
	for my $byte (unpack("C*", $data)) {
		for (0..7) {
			my $cycles = $timing->{'cycles'}->[$byte  &1] / 2;
			my $p0 = $cycles + $delay0;
			my $p1 = $cycles + $delay1;
			if ($adj_count >= $adj_period) {
				$p0 += $adj;
				$adj_count = 0;
			}
			my $period0 = ao_period($p0);
			my $period1 = ao_period($p1);
			print $out_fd sinewave($period0, $period1);
			$sample_count += $period0 + $period1;
			$byte >>= 1;
			$delay0 = $pspec->[2];
			$delay1 = $pspec->[3];
		}
		$pspec = shift if (@_);
		$delay0 = $pspec->[0];
		$delay1 = $pspec->[1];
		$adj_count++;
	}
}

sub sample_out {
	my $samp = shift;
	my $cycles = shift;
	my $period = ao_period($cycles);
	my $fmt = "C";
	if ($bits_per_sample == 16) {
		$samp = ($samp - 0x80) * 0x100;
		$fmt = "s";
	}
	print $out_fd pack($fmt, $samp) x $period;
	$sample_count += $period;
}

sub sinewave {
	my $period0 = shift;
	my $period1 = shift // $period0;
	if (exists $sinewaves{"$period0,$period1"}) {
		return $sinewaves{"$period0,$period1"};
	}
	my $fmt = "C*";
	my $mul = 0x66;
	my $off = 0x80;
	if ($bits_per_sample == 16) {
		$fmt = "s*";
		$mul = 0x6600;
		$off = 0;
	}
	my $sw = pack($fmt,
			(map {
			 my $v = sin((pi*($_-0.5))/$period0);
			 int(($v * $mul) + $off); } (1..$period0)),
			(map {
			 my $v = sin(pi+((pi*($_-0.5))/$period1));
			 int(($v * $mul) + $off); } (1..$period1)));
	#print "$period0,$period1: ".unpack("H*", $sw)."\n";
	$sinewaves{"$period0,$period1"} = $sw;
	return $sw;
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

# Initialise machine code data. Note, does not clear label table.

sub mcdata_org {
	$mc_org = shift;
	$mc_pc = $mc_org;
	@mc_link = ();
	$mc_data = "";
}

# Add machine code data.

sub mcdata_add {
	my $text = shift;
	if (ref($text) eq 'ARRAY') {
		for my $byte (@{$text}) {
			if ($byte =~ /^[a-z]/) {
				$mc_label{$byte} = $mc_pc;
			} elsif ($byte =~ /^\&?</) {
				$mc_pc++;
				push @mc_link, [ $byte, $mc_pc ];
				$mc_data .= "\x00";
			} elsif ($byte =~ /^\&?>/) {
				$mc_pc += 2;
				push @mc_link, [ $byte, $mc_pc ];
				$mc_data .= "\x00\x00";
			} else {
				$mc_pc++;
				$mc_data .= pack("C", $byte);
			}
		}
	} elsif (ref($text) eq 'SCALAR') {
		$mc_data .= $$text;
		$mc_pc += bytes::length($$text);
	}
}

# "Link" the machine code - replace all the entries in @mc_link with computed
# values.

sub mcdata_link {
	for my $r (@mc_link) {
		my $rlabel = $r->[0];
		my $pc = $r->[1];
		my $off = $pc - $mc_org;
		if ($rlabel =~ /^(\&)?([<>])(.*)/) {
			my ($rel,$size,$label) = ($1,$2,$3);
			my $addr = $mc_label{$label} or die "missing label: $label\n";
			$size = ($size eq '<') ? 1 : 2;
			$off -= $size;
			$addr -= $pc if ($rel);
			my $subdata;
			if ($size == 1) {
				$subdata = pack("C", $addr & 0xff);
			} else {
				$subdata = pack("n", $addr & 0xffff);
			}
			bytes::substr($mc_data, $off, $size, $subdata);
		}
	}
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

sub version_text {
	printf "bin2cas.pl %s\n", VERSION;
	print <<EOF;
Copyright (C) 2017 Ciaran Anscomb
License: GNU GPL version 3 or later <http://www.gnu.org/licenses/gpl-3.0.html>.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

Special exception: Code included in output (such as "dunzip") is not
considered to be part of this program, so the GPL doesn't apply to the
results.
EOF
	exit 0;
}

__END__

=encoding utf8

=head1 NAME

B<bin2cas.pl> - convert binary data to CAS or WAV with optional autorun

=head1 SYNOPSIS

B<bin2cas.pl> [I<option>]... I<input-file> [[I<option>]... I<input-file>]...

=head1 DESCRIPTION

Creates CAS or WAV cassette images from Dragon or Tandy CoCo binary input. WAV
output timing are tweaked to target the delays in the ROM tape loading routines
exactly.

Optionally generates complex autorunning code supporting dzip-compressed parts,
faster waveforms, and video mode changing between parts.

=head1 OPTIONS

Output options:

=over

=item B<-o>, B<--output> I<file>

output filename (required)

=item B<-m>, B<--mode> I<mode>

output file mode (cas, wav) [from filename]

=item B<-r>, B<--wav-rate> I<rate>

sample rate in Hz for WAV output [9600]

=item B<--wav-bits> I<n>

bits per sample (8, 16) [8]

=back

Global options:

=over

=item B<-A>, B<--autorun>

build a single autorunning program

=item B<--leader> I<count>

leader length (in bytes) [256]

=item B<--reverse-dzip>

dzipped files are streamed in reverse

=back

Per-file options:

=over

=item B<-B>

file is raw binary (default)

=item B<-D>

file is DragonDOS binary

=item B<-C>

file is CoCo RS-DOS (DECB) binary

=back

=over

=item B<-n>, B<--name> I<name>

name in filename block [from filename]

=item B<-l>, B<--load> I<addr>

load address [0x0000]

=item B<-e>, B<--exec> I<addr>

exec address [0x0000]

=item B<--zload> I<addr>

override automatic load address for dzipped file

=item B<--no-filename>

no filename block required [default when B<--autorun>]

=back

=over

=item B<--no-delay>

no initial delay / leader before data

=item B<--block-size> I<n>

maximum block size [255]

=item B<--no-eof>

don't emit EOF block

=item B<--gapped>

write gapped file

=item B<-T>, B<--timing> I<name>

timing spec (rom, fast, simple) [rom]

=item B<--fast>

alias for B<--timing fast>

=back

=over

=item B<--flasher>

enable cursor flasher in autorun

=item B<-x>, B<--vxor> I<stride>

apply vertical-XOR to file with given stride

=item B<-i> I<file>

input filename (prevents I<file> being parsed as an option if it begins with
"-")

=back

Per-file persistent options:

=over

=item B<-z>, B<--dzip>

dzip file data (dunzip to autorun)

=item B<-E>, B<--eof-data>

EOF blocks allowed to contain data (non-standard)

=back

Inter-file options:

=over

=item B<--lds> I<a>

set the stack pointer to I<a>

=item B<--pause>

insert (additional) half second pause before next file

=item B<--sam-f> I<f>

set SAM display offset to I<f> * 512

=item B<--sam-v> I<v>

set SAM VDG mode to I<v> (0-7, see SAM docs)

=item B<--vdg> I<mode>

set VDG mode before next file

=item B<--poke> I<addr>,I<value>

set one specific byte at I<addr> to I<value>

=back

Other options:

=over

=item B<--help>

show this help

=item B<--version>

show program version

=back

=head1 USAGE

The default sample rate for WAV output is 9600Hz, which has proved to survive
speed variations well when loading on real hardware, while keeping file size to
a minimum. However, if you are going to write the output to tape, it is highly
recommended that you specify a higher rate, e.g., with B<-r 48000>.

Per-file options apply to the next input file. Persistent options apply to all
subsequent files, the rest are reset to defaults after each file.

The B<--autorun> option generates a special loader file containing instructions
for each input file, intended to create a single autorunning program. It
changes the defaults for input files to: B<--no-filename> B<--eof-data>.

When autorunning, the following options apply to the program as a whole, not
just the next file: B<--name>, B<--exec>.

If the B<--omit> option is used, subsequent data parts are not included in the
autorunning loader code.  Useful if handling loading yourself.

To compress data, the "dzip" tool is required to be in your PATH.

Compressed data can overlap the destination, but some non-overlapping clearance
is required depending on the input.  The default is to calculate the clearance
required and otherwise overlap the destination as much as possible.  Sometimes
the extra bytes required stray somewhere inconvenient, so this calculated
address can be overridden with the B<--zload> option. e.g., the extra bytes
beyond a text loading screen encroaches into DOS workspace ($0600), so
B<--zload 0x0e00> would ensure the zipped data was loaded beyond it (usefully,
the end user then doesn't get to I<see> the zipped data loading in this case).

The order of compressed data can be reversed using B<--reverse-dzip>. In this
case, the extra required byte where source and destination overlap must be
I<before> the destination area.  This is probably only useful when data resides
right at the top of RAM. This is a global option: if one dzipped part is
reversed, all are.

For loading screens, "vertical XOR" may help achieve better compression.
Generally, you'll want to specify a stride of 32 (bytes per line).

The default timing spec ("rom") accounts for the slight variations of delay in
the Dragon ROM before it starts to count a pulse width. The "simple" timing
spec is closer to the output of CSAVE. The "fast" timing spec is similar to
"rom", but with shortened cycle widths; code is added to the autorun file to
support this.

Use the B<--flasher> option when the SAM display offset has been changed, e.g.,
during a custom loading screen. The ROM routine will continue to flash $0400,
but this will not be visible, so this adds code to do it manually.

Normally the stack pointer is left at the top of BASIC. Use the B<--lds> option
to adjust it at any point.

=head1 EXAMPLES

A very simple example:

    $0 -o game.cas -D game.bin

Builds a non-autorunning CAS file from the DragonDOS binary F<game.bin>. Load &
exec addresses are taken from the header of the input binary.

A more complex example:

    $0 --autorun -r 22050 -o game.wav --eof-data --dzip --timing fast \
            -B --load 0x0e00 --vxor 32 screen.bin \
            --vdg 8 --sam-v 4 --sam-f 7 \
            -C --flasher game.bin

Builds an autorunning 22kHz WAV file with two parts - a loading screen and the
main game. Each part is dzipped, and a small amount of space is saved by
combining the last data block with the EOF indicator. Vertical XOR is applied
to the loading screen to improve compression. Each part is written using
shorter cycles - just over 30% faster than usual.

The loading screen data is a raw binary, so the load address is specified.
Once the screen has been loaded (including unzipping into place, and reversing
the vertical XOR), the VDG is put into alpha/semigraphics mode with CSS=1, the
SAM is configured to G3C (these two combined configure Semigraphics 12 mode),
and the SAM display offset set to $0e00.

The game code is taken from a CoCo RS-DOS binary which contains load and exec
information. Extra cursor flashing code is added to the autorunner, as the
display offset is no longer $0400. As the last input file, its exec address is
used to autorun once loading is complete.

=cut
