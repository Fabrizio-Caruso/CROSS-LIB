## HOW TO LOAD THE GAME

This document only describes some non-obvious procedures on how to load the game
on some emulators (similar procedures can be followed on the real machines):


-----------------------------------------------------------

### ALPHATRONIC


Use MAME/MESS

mame64.exe alphatro -uimodekey DEL -window -cartridge1 c:\Retro\CROSS-CHASE\build\FULL_alphatro.rom


---------------------------------------------------------

### AMSTRAD CPC/GX4000

You can use WinAPE

- Tape (.cdt) version:
You need to attach the image as a tape
In BASIC type
|tape
run"
Start PLAY on the virtual tape player

- Disk (.dsk) version:
You need to attach the image as a disk
In BASIC type
run"xchase

- ROM (.cpr) version:
You need to attch the image as a cartridge

----------------------------------------------------------

### Amstrad NC 100

Use nc100em-1.2-2014 emulator 
Rename the binary to nc100.card and place it in the same folder as the emulator
Run the emulator. 
Control-X to start the game


---------------------------------------------------------
### ATARI 5200

Use Atari800 emulator with the Atari 5200 bios roms and select Atari 5200 emulation.
Load as 16k one chip cartridge.

---------------------------------------------------------
### ATARI LYNX

Use Handy 0.95 emulator (https://sourceforge.net/projects/handy/?source=typ_redirect)
Place the rom into same folder as Handy emulator (https://atariage.com/Lynx/roms/LynxBoot.zip)

---------------------------------------------------------
### AQUARIUS

Unzip the .zip file.
Type CLOAD 
First load the file starting with underscore
Type RUN
Type CLOAD
Then load the other file


-----------------------------------------------------------
### BIC / A5105

Use JKCEMU
(If not already selected) Einstellungen -> Select BIC 
Datei->Diskettenstation->Laufwerk 1->Open->Diskettenabbilddatei oeffnen->select file .dumb file->Nur lesen
Datei->Schliessen

Run file by its name (e.g., BULD/FU)
Run DIR to see disk content/view/3/4/



-----------------------------------------------------------
### BIT90

Use Mame emulator
e.g., 
mame64.exe bit90 -window -uimodekey DEL

Attach the .rom as a cartridge to load and run the game

-----------------------------------------------------------

### CASIO FP-1100

Use Takeda's fp1100.exe emulator
Attach disk image
Reset


---------------------------------------------------------

### CCE MC-1000

Use the MESS emulator
Use File Manager to attach the WAV file
In BASIC do: TLOAD
Start the tape: Tape Control -> Tape

---------------------------------------------------------

### CBM 610

Use WinVice 3.1 and try multiple times.
Sometimes load"*",8 helps.
There must be a bug in WinVice disk drive emulation for this target.

-----------------------------------------------------------

### COLECOVISION

Use ColEm: https://fms.komkon.org/ColEm/


-----------------------------------------------------------

### COLECO ADAM

Use Mame emulator
e.g., mame64.exe adam -window -flop1 .\disk\adam\cpm2.2_1.50.dsk -flop2 c:\Retro\CROSS-CHASE\build\FULL_coleco_adam.img

B:
Type the binary name (no extension) contained in B:

-------------------------------------------------------------

### Color Computer

- Tape
Use XROAR emulator
Attach the .cas image
Type
CLOADM

Remark: You can also simply use the "run" option


- Disk
Use XROAR emulator (Emulate CoCo DISK EXTENDED)
Attach the .dsk disk image
Type:
LOADM"XCHASE
EXEC


---------------------------------------------------------

### COMX

Use EMMA 02 (https://www.emma02.hobby-site.com/download.html)
Select COMX tab
Click on START (window may hide the option -> make it bigger)
Click on LOAD
Go to the directory with the .hex file
Select HEX Files 
Chose the .hex file
Type RUN
[equivalent to CALL(@4401)]

----------------------------------------------------------

### CP/M (generic system) with ADM-3A control sequences

.COM binaries:

Examples of CP/M systems with ADM-3A support:
multiple Kaypro systems, Aussie Byte (Knight 2000), Decision Mate V

Use MAME emulator with correct roms and CP/M disks
Examples:
mame64.exe kaypro2x -flop1 ..\disk\kaypro\k10hald1.td0 -uimodekey DEL
mame64.exe aussieby -flop1 ..\disk\aussie_byte\knight2000_boot.dsk -uimodekey DEL

WAIT for the operating system to fully load.
Use the quickload option to load the .COM file.

If you want to create a disk image you need to follow the instructions in the Z88DK wiki.

.DSK images:
Use MAME emulator with correct roms and CP/M disks
Examples:
mame64.exe dmv -flop1 ..\disk\dmv\dm5-cpm.td0 -uimodekey DEL

Attach image as disk B:
Run B:
<binari name without .COM>

-----------------------------------------------------------

### CREATIVISION

Use MAME/MESS 
Mess 0.148 with crvision works fine:
mess64.exe crvision -window -cart1 c:\Retro\CROSS-CHASE\build\FULL_creativision_16k_SWAPPED.bin


-------------------------------------------------------------
### DAI

Use Mame emulator
mame64.exe dai -window
Attach tape file
UT (to enter monitor)
R (to load)
G0800 (to start the game)

-------------------------------------------------------------

### Dragon

- Tape
Use XROAR emulator
Attach the .cas image
Type
CLOADM

Remark: You can also simply use the "run" option

- Disk 
Use XROAR emulator (Emulate DOSPLUS)
Attach the .vdk disk image
Type:
RUN"XCHASE.BIN"



-----------------------------------------------------------

### EACA 2000 - EG2K

Use the Genieous emulator: http://gaia.atilia.eu/content/view/3/4/


------------------------------------------------------------

### EMSCRIPTEN
This is a special target: its build is generated by Emscripten and it is targeting browsers.
The build for this target are .html, .js, , .wasm files.

In order to run a game built for this target you need to run an HTTP server:
0. move the generated files inside the emsdk directory
1. python -m SimpleHTTPServer

You can then run your program by using the following URL:
e.g., http://localhost:8000/FULL_emcc_buffered.html

For more info on Emscripten installation:
See https://emscripten.org/docs/getting_started/downloads.html

In particular: 
# Make the "latest" SDK "active" for the current user. (writes .emscripten file)
./emsdk activate latest

# Activate PATH and other environment variables in the current terminal
source ./emsdk_env.sh

Note

On Windows, run emsdk instead of ./emsdk, and emsdk_env.bat instead of source ./emsdk_env.sh.

WSL mounted on:
C:\Users\fabri\AppData\Local\Packages\CanonicalGroupLimited.Ubuntu18.04onWindows_79rhkp1fndgsc\LocalState\rootfs
------------------------------------------------------------

### ENTERPRISE

Use ep128emu (https://sourceforge.net/projects/ep128emu/files/)
Select config EP_128k_EXDOS_FileIO.cfg
Set working directory to the directory with your binary
F1 -> Select binary


----------------------------------------------------------

### EPSON PX-4 / HC-40

Use Takeda emulator for HC-40 with the right SYS.ROM file (https://fjkraan.home.xs4all.nl/comp/px4/).

Rename the binary into BASIC.ROM and replace the original BASIC.ROM that
should be (or have been) in the same directory as the HC-40 emulator executable.

----------------------------------------------------------

### EPSON PX-8 / HC-80

Use Takeda emulator for HC-80 with the right SYS.ROM file.

Rename the binary into BASIC.ROM and replace the original BASIC.ROM that
should be (or have been) in the same directory as the HC-80 emulator executable.


----------------------------------------------------------

### EPSON QC-10 / QX-10

Use Mame emulator
e.g., mame64.exe qx10 -window -uimodekey DEL -flop1 c:\Emulation\TEST\0.196\disk\qc10\EPSNQX10.dsk

Attach the disk image and type the binary name (run DIR to see name).

-----------------------------------------------------------

### BGR COMPUTERS EXCALIBUR 64

Use Mame emulator with the CP/M boot disk
e.g., mame64.exe excali64 -window -uimodekey DEL -flop1 c:\Emulation\TEST\0.196\disk\excali64\excalibur_cpm_system.raw

Attach the game image
B:
Run the binary in B: with its plain name (no extension)

-----------------------------------------------------------

### EXIDY SORCERER

Use Mess
Attach the .wav file
Type LOADG


---------------------------------------------------------

### Galaksija

Use the MESS emulator

Use File Manager to attach the WAV file
In BASIC run: OLD
Start the tape
In BASIC run: RUN


---------------------------------------------------------
### GAMATE

Use MESS/MAME (e.g., Mess 0.148)
Run mess.exe gamate
File Manager -> Select cartridge -> Choose the .bin file


---------------------------------------------------------
### GAMATE

Use Emulitious (https://emulicious.net/)
File -> Open File -> Select .gg file

------------------------------------------------------------

### GRUNDY NEWBRAIN

Use NBemulator15
Create a directory with _dir.txt, a.bas, a.dat 
Insert this directory into the BASIC directory
Start the emulator
File -> Start Emulation
File -> Tape Management -> Select directory with the game's files
Type "LOAD"
Type "RUN"


---------------------------------------------------------
### HOMELAB 2

Use MAME emulator
Use quickload to select the .htp file (and load it into memory)
CALL 17152


---------------------------------------------------------
### HOMELAB 3/4

Use MAME emulator 
Use quickload to select the .htp file (and load it into memory)
CALL $4300


---------------------------------------------------------
### HUEBLER/EVERT-MC

Use JKCEmu
Einstellungen
Select HUEBLER/EVERT-MC
Uebernehmen
Schliessen
Datei -> Lauden -> Select binary
GO 0000


---------------------------------------------------------
### HUEBLER-GRAFIK-MC

Einstellungen
Select HUEBLER-GRAFIK-MC
Uebernehmen
Schliessen
Datei -> Lauden -> Select binary
GO 0000


---------------------------------------------------------
### JUPITER ACE

Use EightOne emulator.

For the FULL version game 
"0 0 bload ace"
Open the FULL .wav version with 
Tools->Wav Loader.

------------------------------------------------------------

### KAYPRO

Use Mame (See CP/M target)

mame64.exe kaypro2x -flop1 ..\disk\kaypro\k10hald1.td0 -uimodekey DEL



---------------------------------------------------------

### Luxor ABC 80 

Use the ABC80SIM emulator (https://www.abc80.org/pub/abc80sim/, https://git.zytor.com/abc80/abc80sim.git/)

.ihx
Pass the ihx file as a parameter to the emulator
Run it directly with the command “A=CALL(49200)”

.bac 
abc80 --casfile program.bac
run cas:

or 

put the .bac file in the right directory (abcdir), 
then run prb:xchase

.dsk
Rename the image to mo0
Put the image in abcdisk
run xchase

or 
Rename the image to mo0
Put the image in <mydir>
Run abc80 -Dd <mydir>
run xchase

Use ABC80Win
Attach .dsk image
load"xchase"
run

---------------------------------------------------------

### Luxor ABC 800

Use the ABC80SIM emulator (https://www.abc80.org/pub/abc80sim/, https://git.zytor.com/abc80/abc80sim.git/)


.bac
abc80 --abc802 --memfile program.bac
Then "run mem:"

.dsk
Rename the image to mo0
Put the image in abcdisk
run xchase

or 
Rename the image to mo0
Put the image in <mydir>
Run abc80 -Dd <mydir>
run xchase

Use ABC80Win
Attach .dsk image
load"xchase"
run
---------------------------------------------------------

### Memotech MTX 500 and 512

Use Bill's MEMU (http://www.primrosebank.net/computers/mtx/emulators/Bills_MEMU_ds.htm)
Place the .mtx binary into the directory "tapes"
Use memu-cfg.exe 
F11 to enter the menu
Select MTX model if necessary (MTX500 or MTX512)
Select tape -> Choose the .mtx binary
Restart



MTX500 binary on MTX512
POKE 64122,0
NEW
LOAD""
RUN

MTX500 binary on MTX500 or MTX 512 binary n MTX512
LOAD""
RUN



Use the MESS 0.118 emulator
Rename FULL_MTX.mtx to FULL and place it in the same folder as the Mess emulator
In Basic do:
LOAD"FULL"
RUN


-----------------------------------------------------------
### MITS Altair 8800

Use
http://www.classiccmp.org/altair32/

Witch it ON
Load the .bin y File -> Read Image File -> Select File -> Address: $0000
Click on STOP/RUN

-----------------------------------------------------------

### MITSUBISHI MULTI 8

Use Takeda emulator
Type "MON"
Then "R"
Select tape file with CMT -> Play -> Select file
Type "GC000"


-----------------------------------------------------------


### MICROBEE

Use Nanowasp emulator: http://nanowasp.org/


-----------------------------------------------------------


### MIKRO-80

Use E3000 emulator http://www.emulator3000.org/e3.htm

Select the binary to load and run it.


-----------------------------------------------------------

### MSX

Use BlueMSX
Attach image.
Run bload"cas:",r


-----------------------------------------------------------

### NICHIBUTSU MY VISION

Use Mame emulator
e.g., mame64.exe myvision -window  -uimodekey DEL

Attach the .rom image file as a cartridge

-----------------------------------------------------------

### NASCOM

Use Virtual-Nascom (http://repo.or.cz/w/virtual-nascom.git)
Type virtualnascom [.nas file]

Once the monitor is running, enter “E 1000” 



-----------------------------------------------------------

### NEC PC-6001

Use Virtual Nec Trek 
Select 2 pages at start-up
Type "cload"
Select cassette image
Type "run"


---------------------------------------------------------
### NES 

Use FCEUX (https://tasvideos.github.io/fceux/web/home.html)

Alternatively Use mednafen and run the following command:
mednafen.exe -fs  1 FULL_nes.nes

---------------------------------------------------------

### Ohio Scientific C1P

Start the WinOSI executable (http://osi.marks-lab.com/)
Under "Options" select "System Configuration...".
Select "OSI C1P / Superboard II (600 BOard)" as machine type.
On the "System ROMs" tab under "Specify ROM Type" select SYN600.
Press "OK"
Under "File" select "Attach Serial Input File...".
Select the file "FULL_osic1p.c1p".
In the emulator window you should see the "D/C/W/M" prompt. 
Press "M" to run the monitor. You then should see the "0000 00" prompt of the machine monitor.
Press "l" (lowercase L). This will load the program and start it.


Memory configs in WinOSI 
Open Osi.ini
32K -> RAMTOP=0x7FFF
8k  _> RAMTOP=0x1FFF

-------------------------------------------------------------

### Olivetti M20

Use Mame
You need to attach the floppy image
mame64.exe m20 -window -flop1 c:\Retro\CROSS-CHASE\build\FULL_m20_buffered.img
At the prompt run with 
aout.cmd


------------------------------------------------------------

### TESLA ONDRA VILI
Use Mame emulator with the ondrav target
e.g., mame64.exe ondrav -window -uimodekey DEL
Press ENTER
Attach .wav image as tape
Start tape to load and autostart the game


------------------------------------------------------------

### ORIC 1 / ORIC ATMOS

Use the Oricutron emulator (https://github.com/pete-gordon/oricutron)
Attach the image.
Run CLOAD""


------------------------------------------------------------

### ORIC TELESTRAT

Use the modified version of Oricutron emulator: http://orix.oric.org/download/
Use the instructions on the link above to boot Orix operating system

Rename the binary file to a short name with no extension
Copy it into sdcard/bin
Run the binary by typing its name 


------------------------------------------------------------

### OSCA 

Use the OSCA emulator (http://www.basoft.co.uk/wordpress/emulation/computer-system-emulation/v6z80p/osca-emulator/)
Use WinImage to inject the .exe binary into a working .img file
Edit the config.txt to make it point to the modified .img file
Run by typing the executable name (whitout .exe)


------------------------------------------------------------

### OZ


Launch vOZ
Drag oz750.rom on the emulator's windows
choose the memory size (256K in my case)
press ENTER (CTRL key) to exit the display settings
press ENTER (CTRL key) again
(optional) Press F6 for brighter display
Drag <program>.wzd on the emulator's windows
Choose the program 'slot' from the emulator's dialog window
Press F5 to select the custom program


-----------------------------------------------------------

### NAMCO PUCMAN (PACMAN)

Use the Mame emulator
You need the puckman.zip rom file.
You have to replace puckmod with the one generated by CrossLib Makefile.
Run Mame:
mame64.exe puckmod -window -uimodekey DEL


-----------------------------------------------------------

### PASOPIA 7

Use Takeda emulator (http://takeda-toshiya.my.coocan.jp/) with the Mame romset

Attach the disk image

Reset to start the game

-----------------------------------------------------------

### PC 8088/8086

Use qemu-system-i386 to start a bare metal PC 8088/8086 and attach the floppy image

qemu-system-i386.exe c:\Retro\CROSS-CHASE\build\FULL_pc8086_turn_based.img


---------------------------------------------------------

### PCE

Use mednafen and run the following command:
mednafen.exe -fs  1 TINY_pce.pce


---------------------------------------------------------

### Pecom 32/64

Use EMMA 02 (https://www.emma02.hobby-site.com/download.html)
Select Pecom tab
Click on START (window may hide the option -> make it bigger)
Click on LOAD
Go to the directory with the .hex file
Select HEX Files 
Chose the .hex file
Type RUN
[equivalent to CALL(&0201)]


---------------------------------------------------------

### Hanimex Pencil II 

Use Mame emulator
Run mame64.exe pencil2 -window -uimodekey DEL
Attach the ROM image


-----------------------------------------------------------

### NEC PC 8801

Use the M88 emulator (https://illusioncity.net/nec-pc-88-series-emulators-tools-lists/)
Type:
MON
R
Select the tape file
G8A00 to start

-----------------------------------------------------------

### PETERS PLUS SPRINTER

Use SPRINT (http://web.archive.org/web/20040905083237/http://shaos.ru/nedopc/sprinter/sprint.htm)
run with something like SPRINTGW.EXE c:\Retro\CROSS-CHASE\build\FULL_pps.exe


---------------------------------------------------------

### Philips 2000

Use m2000-allegro5 emulator
Place the game file into the same directory as the m2000.exe and rename it into p2000.cas
In BASIC execute:
CLOAD
RUN


---------------------------------------------------------
### Philips VG 5000

Type CLOAD
Select the game file


----------------------------------------------------------
### PMD-85

Use Borik's PMD-85 Emulator (https://pmd85.borik.net/wiki/Emulator)
Select the .ptp image file.
Run
MGLD 00
Press the flash button.
Click on the play button.
JUMP 0000


----------------------------------------------------------
### PRIMO

Use Mame
e.g., mame64.exe primoa64 -window  -uimodekey DEL

Use Quickload to load the .pp image


----------------------------------------------------------
### Robotron KC 85/2, 85/3, 85/4, 85/5, HC 900

Use the JKCEMU emulator.
Click on "Extra" tab.
Choose "Einstellungen"
Select your target (e.g., KC 85/5)
Press "Uebernehmen" and then "Schliessen"
Load file with "Datei" -> "Laden" -> Select File -> "Starten"


----------------------------------------------------------

### Robotron Z 1013

Use J1013 emulator (http://www.jens-mueller.org/j1013/download.html):
1. Press Datei->Laden
2. Select binary
3. Press Starten



---------------------------------------------------------

### Robotron Z 9001

Use the JKCEMU emulator.
Click on the "Extra" tab.
Choose "Einstellungen"
Select Z9001
Press "Schliessen"
Load file with "Laden"s

-------------------------------------------------------------

### S-OS

Use sword05 (http://www.retropc.net/ohishi/s-os/sword05.lzh)
Place the file (a.obj) in the same directory as the emulator
(no space)L<SPACE>a.obj
(no space)J 3000



-----------------------------------------------------------

### SAMSUNG SPC-1000

Use ESPC emulator
Type "LOAD"
CMT -> Play -> Select file
CMT -> Play button



---------------------------------------------------------

### SEGA MSTER SYSTEM

Use Kega Fusion Emulator (https://segaretro.org/Kega_Fusion)
File -> Load MasterSystem ROM

---------------------------------------------------------

### SEGA SC 3000

Use the MESS emulator

Run mess with 
mess64.exe sc3000 -cart1 <basic31 file name>.sc -cass1 FULL_sc3000_32k.wav
Inside the emulator type:
LOAD
RUN


---------------------------------------------------------

### Sharp MZ

Use mz800Emu
Press C
Select .mzf file

----------------------------------------------------------

### SHARP X1

Use Takeda X1 emulator (you need the roms)
1. Start the emulator.
2. Click on FD0.
3. Select the disk image


-----------------------------------------------------------

### SHARP PC G850

Use g800win32 emulator (http://ver0.sakura.ne.jp/pc/)
g800.exe -machine=200 c:\Retro\CROSS-CHASE\build\FULL_g800_turn_based.ihx
Type "G100"


-----------------------------------------------------------

### ZX SPECTRUM NEXT
Use CSPECT emulator (http://www.cspect.org/)
Press F2
Select .nex binary to load and start it

-------------------------------------------------------------

### Sinclair QL

Use QPC https://www.kilgus.net/qpc/ 

Rename the binary to something short, e.g., xchase

Type the following commands to add the necessary meta-data and have the binary inside the emulated Sinclair QL
"
f$=“dos1_<path fo file with '_' instead of '/'>“
l=FLEN(\f$)
mem=RESPR(l)
LBYTES f$,mem
SEXEC ram1_<name given to the binary inside the QL>,mem,l,<dataspace value (*)>
"
(*) dataspace has to be big enough, i.e., bigger than what the compiler shows as output.
A possible value is 4096.

Now the ram1 disk has the executable binary inside the QL. Select ram1 and then the game to start


-----------------------------------------------------------


### SONY SMC-77

Use Takeda emulator with Mame roms
Insert the boot disk into the first floppy drive and the game disk in the second one
B:
Run the binary inside B: (no extension)


-----------------------------------------------------------

### SORD M5

Use MESS/MAME
You need the basic-i rom somewhere in your file system
You could run:
mess64.exe m5 -uimodekey DEL -cart1 basic-i\basic-i.rom -ramsize 36k -window
You need to insert the tape
Type TAPE
Press ESC


---------------------------------------------------------

### SUPERVISION

Use Potator (https://emulationrealm.net/downloads/file/235-potator)

Use arrows for movements and C for fire


---------------------------------------------------------

### SONY SV 8000

Use the Mame emulator
e.g., mame64.exe sv8000 -window -uimodekey DEL

Attach the .rom cartridge image to start the game.

---------------------------------------------------------

### DICK SMITH SUPER 80

Use the Mame emulator
e.g., mame64.exe super80 -window -uimodekey DEL

Use quickload to load and start the game

---------------------------------------------------------

### DICK SMITH SUPER 80R

Use the Mame emulator
e.g., mame64.exe super80r -window -uimodekey DEL

Ues quickload to load and start the game

-----------------------------------------------------------

### TATUNG EINSTEIN

USE EDIP1 1e (http://www.einstein.talktalk.net/edip.html?showpage=true)
to insert the .COM file into a disk image:
0. Rename your file such that no special characters (no underscore) are present
1. Run EDIP 1e
2. Select DOS80.DSK
3. Click on CREATE BLANK -> Name your disk image
4. Select DOS TATUNG XTAL...
5. Press SELECT -> Select File
6. Press SELECT -> Cancel -> Quit

Use MAME/MESS:
1. mame64.exe einstein -window -uimodekey DEL -flop1 c:\Emulation\Mame\EDIP1e\XCHASE.DSK
2. run the .COM file by typing its name


---------------------------------------------------------

### Telmac TMC-600

Use EMMA 02 (https://www.emma02.hobby-site.com/download.html)
Select telmac tab.
Select TMC-600
Click on START (window may hide the option -> make it bigger)
Click on LOAD
Go to the directory with the .hex file
Select HEX Files 
Chose the .hex file
Type RUN
[equivalent to CALL(@6300)]


-------------------------------------------------------------

### Thomson Mo5/Mo6/Prodest PC 128

- Tape
Use DCMOTO
Select Mo5 or Mo6 or Prodest PC 128
Attach .k7 image
Type
LOADM
EXEC

-------------------------------------------------------------

### Thomson To7/To8/To9

- Disk .sap image
Use DCMOTO
Select T08D
Attach .sap image
Type
loadm "FULL_TO7.bin"
exec

-----------------------------------------------------------

### TI-82

Use the VTI emulator

1. Load the Crash Shell into the calculator
- 2nd + x,T,\theta (LINK)
- Select Receive
- Drop Crash .82b 
- 2ND + ON (OFF) 
- ON (+ SAVE STATE to start from this point next time)

2. Load the game
- 2nd + x,T,\theta (LINK)
- Select Receive
- Drop .82p file
- 2ND + ON (OFF) 
- ON (+ SAVE STATE to start from this point next time)


3. Run the game
- PRGM
- Select Crash (NOT the game)
- Select the game


-----------------------------------------------------------

### TI-83

Use the VTI emulator

1. Load the Ion Shell into the calculator
- 2nd + x,T,\theta,\pi (LINK)
- Select Receive
- Drop .83b Ion 1.5 or 1.6 file
- 2ND + ON (OFF) 
- ON (+ SAVE STATE to start from this point next time)

2. Load the game
- 2nd + x,T,\theta,\pi (LINK)
- Select Receive
- Drop .83p file
- 2ND + ON (OFF) 
- ON (+ SAVE STATE to start from this point next time)

3. Run the "a" application
- PRGM
- Select A 

4. Run the game
- PRGM
- Select Ion (NOT the game)
- Select the game


Alternative load instructions:

Use VTI 2.5 beta 4 or beta 5.
Select TI83 (v1.07)
Reset calculator (MEM CLEARED)
2nd, LINK, RECEIVE (do not press ENTER, yet)
Look for ION.83G (memory dump)
Press ENTER on ION
Drag the file onto VTI
Transmission error. Retry.
PRGM -> ION (see prgmION) -> press Enter (INSTALLED)
Compile your program with a name different from default abc80
Transfer program as done with ION.83G
Start the shell and program


-----------------------------------------------------------

### TI-85

Use the VTI emulator

1. Load the Rigel Shell into the calculator
- 2nd + X-Var (LINK) + F2 
- Drop Rigel memory backup file
- 2ND + ON (OFF) 
-  ON (+ SAVE STATE to start from this point next time)

2. Load the game
- 2nd + X-Var (LINK) + F2 
-  Drop the .85s game binary

3. Run the game
-  Custom + F1 (to select Rigel)
- Select the game

----

Alternative instructions:

    Install the emulator program VTI.
    Download the TI85.ROM and put it into VTI directory.
    Download and extract Rigel Shell.
    Start VTI. Select TI85.
    Click 2nd Link, then press F2 to receive.
    Right Mouse click and choose Send File to VTI. Choose the file rigel.85b from the Backups directory of the Rigel Shell.
    Quickly press F1 to confirm Warning about Memory backup, before transmission error message.
    Click 2nd OFF On, to init the Rigel Shell.
    Click 2nd Link, then press F2 to receive.
    Right Mouse click and choose Send File to VTI. Select spice.85s.
    Click 2nd OFF On, to init the Rigel Shell.
    Click Custom, then F1, and select SPICE program.


QUICK
	Load Rigel savestate 
	Click 2nd OFF On
    Click 2nd Link, then press F2 to receive.
    Right Mouse click and choose Send File to VTI. Choose the .85s file
    Click 2nd OFF On, to init the Rigel Shell.
    Click Custom, then F1, and select A program.	
	
		
	
-----------------------------------------------------------

### TI-86

Use Wabbitemu

You can load the .86p files by directly dragging them on VTI, 
no need to go through the 'link" sequence, they'l be pushed in the TI86 memory.
So, first drag InstLASM.86p, then enter:
asm(InstLASM)
press F5 and confirm the reset process
once you get the message "MEM CLEARED", 
drag LargeLd.86p on the calculator.
Now you can upload ONE SINGLE "big" executable (built with the LASM startup option), just drag it on the calculator.
To run: asm(LargeLd)

-----------------------------------------------------------

### TIKI-100

Use TIKI-100_emul (http://www.djupdal.org/tiki/emulator/)

Attach the image and type: 
a 
folowed by enter.


----------------------------------------------------------

### TRS-80 Model 1/2/4

.cas version:
If you use trs80gp-1.8
Type enter to get to the console
Use load option

If you use trs32

.cas version:
Use trs32 emulator.
Type L+enter at Cass?
Type enter at Memory?
Type System
Attach .cas
Show cassette
Type ../BUI
Press PLAY
Wait...
Press / + enter to start

.cmd version:
Get to BASIC prompt
File->Run-> Select .cmd file


----------------------------------------------------------

### VIDEOTON TVC
 
Use WinTVC emulator (http://gaia.atilia.eu/downloads/)
Start the emulator and wait until the BASIC prompt
Load the .cas file with "CAS direkt betoeltese"
Type "run" to start


-----------------------------------------------------------


### VECTOR 06C

Use E3000 emulator http://www.emulator3000.org/e3.htm

Select the binary to load and run it.


----------------------------------------------------------

### VTECH Laser 500

Use the online Laser500emu (https://nippur72.github.io/laser500emu/)
Drop the .bin or .wav file onto the browser window


----------------------------------------------------------

### Z80 TV GAME
Use Takeda emulator with Mame roms

Start the game: Cart -> Insert -> Select .rom image file 
Input: Host -> Input -> Joystick1 -> Redefine keys



