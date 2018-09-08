ImageDisk
---------
ImageDisk is a program to read entire diskette images into files (type .IMD),
and to recreate a copy of the diskette from that image. A detailed analysis
is performed on the diskette, and information about the formatting is recorded
in the image file. This allows ImageDisk to work with virtually any soft-
sectored diskette format that is compatible with the PC's type 765 floppy
diskette controller and drives.

The ImageDisk Disk Image File (.IMD) format specification has been released
into the public domain and I wish to encourage others to write software which
creates and manipulates .IMD files, especially under other operating and/or
host systems.

The executable ImageDisk program, utilities and complete documentation are
available in another archive - for general information on the program, please
refer to this material.


ImageDisk Source Code
---------------------
This archive contains the full ImageDisk source code, including the main
IMD program as well as the AMDA, IMDU, IMDV, TESTFDC, BIN2IMD, TD02IMD,
DMK2IMD and ANY2IMD utilities.


                            Terms and Condtiions

The ImageDisk source code consisting of the files IMD.C, IMDA.C IMDU.C,
IMDV.C, TESTFDC.C, BIN2IMD.C, TD02IMD.C, DMK2IMD.C and ANY2IMD.C is copyright
material, and is licensed under the following terms and conditions:

-   No support, technical or otherwise is offered for the source code.

-   Use of source code is entirely at your own risk. The author makes no
    guarantee of the suitability of this code for any particular purpose.

-   The code is NOT "open source", and is NOT being offered under GPL or
    any other "open source" license. The code is for personal use only.

-   You may use the ideas and algorithms expressed within this code in
    your own software with no restriction.

-   You may NOT distribute this source code or significant portions
    thereof in any form. You may direct third parties to my web site
    where I will maintain the latest version of the source code.

All rights and permissions not listed above are explicitly reserved by the
author.

Please note:

The main intentions of these license terms are:

-   To avoid a high support load on myself.

-   To avoid multiple/conflicting public versions of the PC/DOS
    ImageDisk package.

-   To allow me to remain informed on other ImageDisk developments.

-   To insure the program remains non-commercial.

If you wish to enhance the PC/DOS version of ImageDisk or develop it for
another platform, please contact me - I am very accomodating.


Additional notes:
-----------------
This code was developed using my own Micro-C/PC compiler. Micro-C/PC is
available free from my commercial site www.dunfield.com. This is not a
mainstream toolset and some features and library functions work differently
than you may expect. ImageDisk makes use of non-standard features and library
functions and will not compile under a different toolset without significant
modification. Please do not contact me for assistance in porting this code to
another toolset - I maintain the PC/DOS version of ImageDisk for my tools only.

If you do compile ImageDisk under Micro-C/PC, please note that some assemblers
(notably MASM) cannot handle the large output file it generates. You will
either have to break the program down into smaller source files, or use
a different assembler (Borlands TASM 1.0 works well for me).

The source code files are written assuming tabs occuring at 4 character
intervals - these will display with incorrect spacing if you edit the files
with an editor set to the standard MSDOS tabs at 8 character intervals.
For those who do not have an editor which can handle tabs at 4 character
intervals, I have included a program REMTAB.COM which will replace all tabs
in the source files with the correct number of spaces to retain proper
formatting.

ImageDisk is a "created by necessity" program which deals with a difficult
problem (accessing non-standard floppy disk formats on widely varying PC
hardware). It was initially hacked together in a weekend, and subsequently
bludgeoned into it's current form over many months of my own experience with
it as well as user reports of it's strengths and weaknesses. Within the source
code you will find gotos, casted pointers, inline assembly language and other
things some people don't like - if you find these offensive, strongly disagree
with my coding style, or otherwise have difficulty with the source code -
please delete it now. If you have legitimate improvements, or bug reports -
I'm all ears, but I have neither time nor desire to haggle over matters of
personal opinion.

Enjoy,

Dave Dunfield

To contact me, see the "contact me" link on my classic computers website:
    www.classiccmp.org/dunfield

or use the "technical support" link on my commercial website:
    www.dunfield.com

