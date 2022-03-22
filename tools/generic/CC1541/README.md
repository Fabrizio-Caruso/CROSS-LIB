# README #

This is cc1541, a tool for creating Commodore 1541 Floppy disk
images in D64, D71 or D81 format with custom sector interleaving
etc.  Also supports extended tracks 35-40 using either SPEED DOS or
DOLPHIN DOS BAM-formatting. Furthermore supports writing Transwarp
disk images for the fantastically fast loader by Krill.

Originally written by JackAsser, with improvements and fixes by
Krill, some more improvements by Claus and further improvements by
Björn Esser.

The program is provided under the terms of the MIT license, please
refer to the included LICENSE.txt file for its terms and conditions.

The public source code repository can be found here:
https://bitbucket.org/PTV_Claus/cc1541/src/master/

## Usage examples ##

* "cc1541 -h" shows the command line help
* "cc1541 image.d64" lists the content of image.d64
* "cc1541 -f program -w program.prg image.d64" adds the file
  program.prg to image.d64 (or creates the image if it does not
  exist yet) using the name "program"
* "cc1541 -f program1 -w program1.prg -f program2 -w program2.prg
  image.d64" adds two files under the names program1 and program2
* "cc1541 -s 4 -f program -w program.prg image.d64" writes a file
  with a dedicated sector interleave for a fastloader (the best
  value depends on the used fastloader and its configuration)
* "cc1541 -f game -W game.prg -f 'transwarp v0.86' -w transwarp.prg
  Transwarp-Game.d64" creates a Transwarp disk image with the
  required boot file.
* "cc1541 -T DEL -f ---------------- -L image.d64" creates a DEL 
  entry as separator.

## Version history ##

v3.4

* Support for Transwarp 0.86 and later
* Commandline option -R added for restoring deleted or formatted
  files
* -L switch added to create directory entries without writing a 
  file
* -a switch added to print a cc1541 command line that re-creates
  the directory art of the given disk image
* -T now also allows a number as parameter
* Files with file type DEL will now actually be written, use
  -L if you do not want that
* Improved verbose allocation printout
* More robustness against invalid t/s links
* Added support for underscore in PETSCII filenames
* Bugfix: no endless loops for cyclic directory chains anymore

v3.3

* Transwarp encoding added
* Verbose mode displays file allocation on unmodified images
* Optimised non-standard interleave scheme
* Added unicode printout option for PETSCII, special thanks to
  Jonas Minnberg for helping with Linux support and inverse
  characters!
* Output file is not touched when no change is requested
* Bugfix: Fixed crash on write protected output file
* Bugfix: Spacing in directory printout was not correct when
  file size is more than 999 blocks

v3.2

* -N switch added to allow duplicate file names
* Trying to create loop file with the same name as the referenced
  file now throws an error, if not -N is also specified
* Changed default next file first sector on new track to 0 to align
  with Commodore DOS
* Interleave violations are now shown in block allocation printout
* Block allocation for loop files is not displayed anymore when
  referenced file was also written
* Bugfix: Fixed file index printout for block allocation
* Bugfix: Correct interleave is now displayed for loop files when
  referenced file was also written

v3.1

* Bugfix: overwriting files did not work correctly when there was a
  free slot in the directory before the file to overwrite
* Bugfix: Filetype, open and protected flags were not reset after
  writing a file
* Bugfix: default disk name and id were wrong in command line help

v3.0

* ASCII to PETSCII conversion added, this breaks backward
  compatibility (and therefore warrants a major version increase)!
* Support for D81 images
* Default printout is now a full directory similar to how it would be
  displayed on a Commodore machine
* -v switch added for verbose output of file and block allocation
* -M switch added to specify number of characters for filename hash
  calculation for latest Krill loader
* -m switch added to skip filename collision check
* -B switch added to allow setting the displayed file size
* -o switch added to prevent overwriting of existing files on an image
* -V switch added to validate images before editing them
* -T switch added to allow setting the file type
* -O switch added to allow setting the open flag
* -P switch added to allow setting the protected flag
* Hex escapes are now also allowed for disk name and ID
* When no disk file name is provided, only the base name of the input
  file is used as disk file name instead of the full path
* Bugfix: fixed memory access issue for filenames with 16 letters
* Bugfix: fixed endless loop when reaching track 53 on a D71
* Bugfix: fixed that (shadow) directory track would be used for data
  when the shadow directory is a neighbour of the actual directory
* Bugfix: G64 output is now an optional additional output using -g,
  avoiding the utterly broken reading of G64 files
* Bugfix: loop files have actual file size per default instead of 0
* Bugfix: printouts to stderr and stdout are more consistent now

v2.0

* The first version with a release number
* All existing modifications consolidated (hopefully)
* G64 output dependent on output file name instead of a source code
  define
* Converted to ANSI C99
* MSVC build files added
* getopt removed
* Simple test suite added
* Bugfix: hex escape was not considered for file overwrite detection
* Bugfix: first sector per track was ignored for track 1
* Bugfix: default sector interleave was ignored for first file
