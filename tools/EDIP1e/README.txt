IN THE ZIP ARCHIVE

EDIPv1e.exe EDIP version 1e

EDIPV1e.txt listed updates,

Extension_Trim.exe
This program simply renames a file to have only thee
characters for its extension.
e.g. An Einstein file extracted by EDIP
D:\Einsteinfiles\Disc1\COPY   .COM_EDIPv1e_XTAL
to
D:\Einsteinfiles\Disc1\COPY   .COM
Other files which only have three character extensions will not
be altered.
Caution with other MS files
i.e. *.html will be changed to *.htm

TESTlge.txt A text file used for testing Edip extraction
and insertion with a HEX editor.
When viewed in a Hex editor displaying offset, 16 bytes and ascii text.
A text value should match that of the offset.
0x0000 3030 3030 424C 3043 4B2D 3030 3030 0D0A 0000BLOCK_0000..
0x0010 3030 3130 2D2D 2D2D 4254 2D2D 2D2D 0D0A 0010____BT____..
0x0020 3030 3230 2D2D 2D2D 4254 2D2D 2D2D 0D0A 0020____BT____..
  |..|                 same as                 |..|

The last line will show an offset of 0x8800.
If the file is inserted into a disk image by EDIP
then a copy of TESTlge.txt is extracted from the image
the new file will be increased in size rounded up to
the nearest 128 byte sector. This complies to the CP/M
operating systems. Additional bytes having the value
&00. This is correct any thing else will show
corruption when viewed with an HEX editor.
The last line of the extracted file will show an offset
of 0x8870.

In the Einstein if the Xtal DOS LOAD TESTlge.txt command
is used then MOS tabulate command used the offset should
show a value a &100 greater than that shown in the text
part of the table.



