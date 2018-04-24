CPCXfs 20-August-2010
========================
CPCfs (c) Derik van Zuetphen
CPCXfs by Kevin Thacker. Based on CPCfs v0.85
22DISK ".DEF" file created by Frederic Herlem.

** This is not an official CPCfs release. **

I was talking to Frederic Herlem, maintainer of the ALL-CPC list,
and he mentioned bugs he had discovered and features he would like to see in CPCfs.
We both agree that CPCfs is an excellent utility for transfering files to
and from CPCEMU style disk images.

Since I have not seen any CPCfs releases for a while, I decided to take
the original CPCfs source and add my changes to fix a few bugs and add
some more features.

Frederic has tested this version quite thoroughly so there should not be many bugs.
We have both tried hard to remove all major bugs (crashes, corruption of disk images).

If you find any bugs, please e-mail me with details to reproduce
the problem. 

I hope that eventually my source additions will be part of the official distribution.
Derik, if you are reading this, please feel free to use any part of my changes in CPCfs.

Included in this release is a Windows Command-line executable (cpcxfsw.exe)
 - which will run in a DOS box under Win95/98, and on the Command Prompt under Win2000 and
WinNT, and a DOS version (cpcxfs.exe) - compiled with DJGPP C compiler.

If you want to run cpcxfs in linux, please ensure all files are in lowercase,
otherwise they might not be found.

I have released this source for the benefit of others. All credit goes to Derik for
his original CPCfs code. CPCxfs is *not* my own work. 90% of CPCXFS is the work of Derik,
with 10% from my bug-fixes and additions.

I hope that with the permission of Derik we could all help to extend the 
functionality of CPCfs furthur. 

I would prefer that CPCxfs would be integrated into the original CPCfs.

The included "cpmdisks.def" has been checked and compiled by Frederic.
I have included the ZX Spectrum + 3 additions from Cristian Secara.

It is compatible with 22disk ".def" files and can be used with 22disk without
modification. It has been compiled from a few ".def" files from the great Amstrad CPC archive at:
ftp://ftp.lip6.fr/pub/amstrad

You can customize this ".def" file by adding and removing definitions. If you create a new
definition we would be happy to add it to the next distribution of cpcxfs.

There are two built-in default formats "DATA" and "SYSTEM", these will exist if a ".def" 
file is not found.

I have renamed this version of CPCXfs so people will know it is not an official public release 
of CPCfs.

I hope you will enjoy this update.

Best Wishes,

Kevin
amstrad@aiind.upv.es

Description of extra files in this release
==========================================

fred.lst	=	List of formats in .DEF file
cpmdisks.def	=	22disk .DEF file (defines disk formats CPCXFS can use)
src/readdef.c	=	Source code to read and parse .DEF files
pcw3.lbl        =       system boot block/system information block for pcw3 format
zx0.lbl/zx3.lbl =       system boot block/system information block for Spectrum +3

Changes for 20-August-2010 release:
===================================

Attrib command didn't mark the dsk as dirty. The result was that if this command was used via the
command-line then the dsk would not be changed.

Paths and filenames that had spaces and that were quoted did not work from the command-line.

Source now includes a project for Code:Blocks (cpcxfs.cbp)

Changes for 22-January-2006 release:
=====================================

These changes were made last year but I haven't released the changes until now.

A few bug fixes including handling of multi-side discs using EAGLE or CYLINDERS sidedness.


Changes for 16-February-2003 release:
=====================================

- bugfix: mget would cause a crash if a image was not loaded. Now fixed.

- changed: when CPCxfs is used from the command-line in non-interactive mode, any directory listing
taking more than one page will no longer ask for a key to be pressed to go to the next page. This means
that batch files and scripts can be more automatic with less user interaction.

- changed: "auto" file type detection is not great and sometimes recognises a binary file as text. Default
mode changed to "binary".

Changes for 12-January-2002 release:
=====================================

- bugfix: bad sector id's were generated with some skew values (e.g. 3). Bug reported by Cristian Secara.
  Now fixed.

- bugfix: pressing CTRL-Z in Windows version caused the command-line to repeat until CTRL-C
  was pressed. Now fixed.

- removed ams1.lbl and ams2.lbl and replaced with pcw3.lbl, zx0.lbl and zx3.lbl. These files
  supplied by Cristian Secara. Thankyou!

Changes for 02-September-2001 release:
======================================

- "sysgen" command removed. This command did not work correctly with the formats
defined by the .def file. Use the new "label" command.

- added "label" command. This command can be used to install a boot-sector, or boot
program into the system tracks of the disc. 

- "label" keyword in .def now supported. This has the same function as the "label" command.
if "label" is found in the def, a file will be pre-loaded and this will automatically be
applied to the disc image after formatting. Thankyou to Christian Secara for suggesting I add this.

- added ZX Spectrum +3 format definitions to cpmdisks.def file. supplied by Cristian Secara

- added ams1.lbl and ams2.lbl label files used by PCW and ZX Spectrum +3 disk formats. supplied by Cristian Secara.

- bugfix: a track that was modified was not always written back to the image. now fixed.

Changes for 28-October-2000 release:
====================================


- added "clean" command. This command will nuke all unused blocks, and nuke all directory
entries with deleted file information in them. There is no undo for this operation!
Use with care! This command is the best for creating clean images. These clean images will
compress well with zip.

A block is classified as unused if it is not used by any valid file in the directory.
A valid file has a user number not equal to 0x0e5.


- Bug-fix: the dos version, under DJGPP, would generate a bad temporary filename 
("dump" command).


- modified the code slightly to compile with DJGPP for DOS.

- edited makefile. It can be used for dos and linux versions.
If you want to compile linux version, remove the "#" from the LINUX_VER = 1
line and add a "#" to the DOS_VER = 1 line.
If you want to compile dos version, add a "#" to the line with LINX_VER = 1,
and remove the "#" from the DOS_VER = 1 line.


- format/new command can create an extended disk image. Use "-e" option.
Documentation changed for this new parameter.

- format/new command uses "skew" from .def. The disk images are created with skewed sector index's.


- Bug fix: Sometimes, the cpcxfs install directory could not be identified. 
(argv[0] was only the executable name!). In this case, if the directory had been changed
from the install directory, "help" would not work and "cpcfs.hlp" could not be found.

- changed documentation to remove references to vortex and ibm formats in the open command.
Now there are only two standards, the others can be added using a .def file

- changed "map" command to handle 16-bit block numbers.
Allocated directory blocks appear as "DDDD".
Tracks/Sectors which cannot be used, because the total number of blocks is to small,
are shown as "-NA-". (e.g. a disc image with 60 tracks used, but not all the sectors
of the disc are used by blocks).
System blocks are marked as "$$$$" as before and free blocks as
"----".

- added code to handle different first sector id for each side of the disc.
"DiskPara 824K Format 83trk 10sct 192dir 2Kpb Two Sides" supported.

- when "formats" command is used, it will display a page at a time. You can press "c" to show
the next page.

- implemented code to support EXM parameter in .def file. Formats using EXM should
work now. EXM=0, EXM=1, and EXM=3 are known to work, other EXM values may not work please
let me know if they do not.

- added code to store head index from SIDE1 and SIDE2 parameter. This is used in the format
command to initialise a disk image, and to access the sectors in the disc image.
(AD2B format is useable now)

- changed alloc_block to use a bit-array to mark allocated blocks. previous code
didn't work properly if more than 254 directory entries were used by the disc format.
(This was perfectly ok for data and system formats, but since I added support for
more formats, this became a problem!)

- dump code changed to generate the block allocations from the files in the directory
(deleted files not included)

- uses AL1 and AL0 to mark blocks that are reserved/allocated (reserved blocks include
the directory blocks)

- added a directory_dirty variable. This is used to keep track if the directory
has been modified and needs writing back to the disc.
this should stop cpcfs from corrupting disc images

- implemented support for ORDER parameter in def file. SIDES, CYLINDER and EAGLE are
all supported. 

- bug-fix: would crash if more characters were entered than input buffer could handle.
Now fixed.

- bug-fix: It was assumed that the block list occupied by the file (bytes 8-15 of the
directory entry) would be full (i.e. 16 byte-indexs or 8 word-indexs). This is not true for some disc images,
which use a maximum of 8-byte indexs per entry. The number of byte-index's or word-indexs,
is dependant on a number of factors including EXM.

- bug-fix: if a image was abandoned, the file was not closed! Now the file is closed.
Added a common "free_image_data" function called by close and abandon, to free buffer's
close the file and re-initialise some data.

- bug-fix: if a error occured when a file was not fully written (e.g. disk full), the
blocks would not be correct (they would be 0x0e5). This produced wrong file sizes
when used in an emulator.

- bug-fix: the default definitions for DATA and SYSTEM had incorrect DSM values.
The space available was 4k too small!

- bug-fix: there were translation bugs in the lcd command. e.g.
If the current working directory was "F:\" and I tried lcd "C:\" it would
not change to C drive. 

- bug-fix: fixed some bugs in the handling of .def files where CYLINDERS defined a 
different number of tracks from the number of tracks calculated from DSM.

- system and data default formats already available

- support for Extended disk image file format

- changed code so that it could be compiled using Microsoft
Developer Studio. I only did this because I prefer Microsoft Developer
Studio to develop with. A Windows command-line version can now be created
which will work under WinNT, Win95, Win98 and Win2000.

- support for 22disk .def file. CPCfs used to have a fixed selection
of formats, now it can be configured.

- support for many disk format types (Romdos, Amsdos, Vortex etc),
through a 22disk .def disk format definition file.

- improved disk format detection, compared to my previous update.

- added "formats" command which lists the available formats.
The list takes the form:

<identifier> (<description>)

- added "nuke" command. This will delete a file - the file cannot be undeleted after
using this command!
The directory entries are cleared to 0x0e5 completely, and all blocks used by the file
are cleared to 0x0e5. This command is ideal to produce cleaner disc images. It is
useful because the disk images can be compressed better. Use with care!

nuke <filespec>

- changed format command:
format -f <format identifier> <disk image name>

The <format identifier> is the <identifier> listed in the "formats" command.
Please type the identifier exactly and in the case shown in the list.

or:

format -d <disk image name>

to format with DATA format. 

format -s <disk image name>

to format with SYSTEM format. This is a default format.

- updated stat command:
This will now display the disk image type ("standard" or "extended"), and will
report the detected format in the same way as it is seen in the "formats" command.

- changed open command
OPEN -f <format name> <diskimagename.dsk>

will force the format specified (if available), and open the disk image.
if:
OPEN <diskimagename.dsk>

is used, format will be auto-detected.
CPCfs will list the formats it has found that match the format of the disk image.
If more than one format is found, CPCfs will not open the disk image. In this
case the user must force the disk image format of choice.

- improved the disk image validation code. There is less chance of a bad image
from being inserted and crashing CPCfs.

- added more checking to some functions to stop CPCfs from crashing.

- bug-fix: disk images where sectors were not stored consecutive now work.
(after reading the documentation this appears to be intentional. In my opinion,
this means that CPCfs is restricted in the disk images that can be used. A disk image,
in DATA format would fail if the sectors were not consecutive, even if the whole
image did not contain copyprotection!).

- bug-fix: fixed possible problems with long filenames. The end of some buffers
could be overwritten causing crash problems. Not likely under DOS but could happen
under Linux and Windows version. The filenames will be cut if they are too big for
the buffers, so you may experience problems if this happens.

- added sanity checks in places where crashes could occur.

- this version will compile with linux. Thankyou to Ewan McNeill for these changes,
and extra bug-fixes. 

To be done:
===========

- choose disk image format to write (extended/standard)
- anything else anyone can suggest...
 
