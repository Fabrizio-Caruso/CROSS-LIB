## STATUS
Last update: 2024/05/11

### 8-BIT targets with known working existing emulators


target name         | long target name                            | working  |   CPU family  | dev-kit | real-time input | hires graphics |  sound  | notes      
--------------------|---------------------------------------------|----------|---------------|---------|-----------------|----------------|---------|-----------
*abc80*             | Luxor ABC 80                                |   YES    |   Zilog 80    |  Z88DK  |      OK         |    missing     | missing |
*abc800*            | Luxor ABC 800                               |   YES    |   Zilog 80    |  Z88DK  |      OK         |    missing     | missing |
*ace*               | Jupiter Ace                                 |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    |
*alphatro*          | Alphatronic                                 |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A       | missing |
*altair8800*        | MITS Altair 8800                            |   YES    |  Intel 8080   |  Z88DK  |     N/A         |      N/A       |  N/A    |
*apple2*            | Apple II                                    |   YES    |   MOS 6502    |   CC65  |      OK         |       OK       |   OK    |
*apple2enh*         | Apple //e                                   |   YES    |   MOS 6502    |   CC65  |      OK         |       OK       |   OK    |
*aquarius*          | Mattel Aquarius                             |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A       |   OK    |
*atari5200*         | Atari 5200                                  |   YES    |   MOS 6502    |   CC65  |      OK         |       OK       |   OK    |
*atari7800*         | Atari 7800                                  |   YES    |   MOS 6502    |   CC65  |      OK         |    missing     | missing |
*atari*             | Atari 8-bit (e.g., Atari 800)               |   YES    |   MOS 6502    |   CC65  |      OK         |       OK       |   OK    |
*atari_lynx*        | Atari Lynx                                  |   YES    |   MOS 6502    |   CC65  |      OK         |       OK       |   OK    |
*attache*           | Otrona Attache                              |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       | missing |
*aussie*            | Aussie Byte (Knight 2000)                   |   YES    |   Zilog 80    |  Z88DK  |      OK         |    missing     |   OK    |
*bbc*               | BBC Micro                                   |   YES    |   MOS 6502    |   VBCC  |    missing      |    missing     | missing |
*bbcmaster*         | BBC Master                                  |    NO    |   MOS 6502    |   VBCC  |    missing      |    missing     | missing |
*bic*               | BIC/A5105                                   |   YES    |   Zilog 80    |  Z88DK  |      OK         |        ?       |    ?    |
*bit90*             | Bit Corporation BIT 90                      |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       | missing | PSG SN76489A
*bondwell*          | Bondwell                                    |   YES?   |   Zilog 80    |  Z88DK  |      OK         |        ?       |    ?    |
*bondwell2*         | Bondwell 2                                  |   YES?   |   Zilog 80    |  Z88DK  |      OK         |        ?       |    ?    |
*c128_8502*         | Commodore 128 [8502 CPU]                    |   YES    |   MOS 6502    |   CC65  |      OK         |    missing     |   OK    |
*c128_z80*          | Commodore 128 [Z80 CPU]                     |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    |
*c16*               | Commodore 264 series (16/116/+4)            |   YES    |   MOS 6502    |   CC65  |      OK         |       OK       |   OK    |
*c64*               | Commodore 64                                |   YES    |   MOS 6502    |   CC65  |      OK         |       OK       |   OK    |
*cbm510*            | Commodore CBM 510                           |   YES    |   MOS 6502    |   CC65  |      OK         |    missing     | missing |
*cbm610*            | Commodore CBM 610                           |   YES    |   MOS 6502    |   CC65  |      OK         |    missing     | missing | 
*camputers_lynx*    | Camputers Lynx 48k/96k/128k                 |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       | missing |
*cidelsa_altair*    | Cidelsa Altair arcade board                 |   YES    |   RCA 1802    | LCC1802 |      OK         |    missing     |   OK    |
*cidelsa_destroyer* | Cidelsa Destroyer arcade board              |   YES    |   RCA 1802    | LCC1802 |      OK         |    missing     |   OK    |
*cidelsa_draco*     | Cidelsa Draco arcade board                  |   YES    |   RCA 1802    | LCC1802 |      OK         |    missing     |   OK    |
*coco*              | Color Computer 1/2                          |   YES    | Motorola 6809 |   CMOC  |      OK         |       OK       |   OK    |
*coco 3*            | Color Computer 3                            |   YES    | Motorola 6809 |   CMOC  |      OK         |       OK       |   OK    |
*coleco*            | Colecovision                                |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       | missing | PSG SN76489A
*coleco_adam*       | Colecovision Adam                           |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       | missing | PSG SN76489A 
*comx*              | COMX-35                                     |   YES    |   RCA 1802    | LCC1802 |      OK         |       OK       |   OK    | 
*corvette*          | Corvette                                    |   YES?   |   Zilog 80    |  Z88DK  |      OK?        |       OK?      | missing |
*cpc*               | Amstrad CPC                                 |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    |
*cpm_8080*          | CP/M Intel 8080                             |   YES    |  Intel 8080   |  Z88DK  |      OK         |      N/A       |  N/A    |
*cpm_z80*           | CP/M Z80 with adm3a/vt52/vt100              |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A       |  N/A    | 
*creativision*      | VTech Creativision                          |   YES    |   MOS 6502    |   CC65  |      OK         |       OK       |   OK    |
*cx16*              | Commander CX 16                             |   YES    |   MOS 6502    |   CC65  |   missing       |    missing     | missing |
*dai*               | DAI                                         |   YES    |  Intel 8080   |  Z88DK  |   missing       |      N/A ?     |  N/A ?  |
*dmv*               | Decision Mate V                             |   YES    |   Zilog 80    |  Z88DK  |      OK         |        ?       |    ?    |
*dragon*            | Dragon 32/64                                |   YES    | Motorola 6809 |   CMOC  |      OK         |       OK       |   OK    |
*eg2k*              | EACA EG2000 Color Genie                     |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK ?  |
*einstein*          | Tatung Einstein                             |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK ?     |   OK ?  | PSG AY-3-8910
*enterprise*        | Enterprise 64/128                           |   YES    |   Zilog 80    |  Z88DK  |   missing       |    missing     | missing | 
*excalibur64*       | Excalibur64                                 |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A ?     |   N/A ? |
*fp1100*            | Casio FP-1100                               |   YES    |   Zilog 80    |  Z88DK  |      OK         |    missing     |   N/A ? |
*g800*              | Sharp PC-G800                               |   YES    |   Zilog 80    |  Z88DK  |   missing       |    missing     |   N/A ? |
*gal*               | Galaksija                                   |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A       |   N/A   |
*gal_plus*          | Galaksija Plus                              |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK ?     |   N/A   |
*gamate*            | Gamate                                      |   YES    |   MOS 6502    |   CC65  |      OK         |       OK       |   OK    |
*gamegear*          | Game Gear                                   |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK ?  | PSG SN76489 Mednafen OK 
*gb*                | Game Boy                                    |   YES    |   GBZ80       |  Z88DK  |      OK         |       OK       |   OK    | grey levels
*gemini*            | Gemini                                    |   YES?   |   Zilog 80    |  Z88DK  |      OK         |        ?       |    ?    |
*gx4000*            | Amstrad GX-4000                             |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    |
*hemc*              | Huebler/Evert-MC                            |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A       |   N/A ? |
*hgmc*              | Huebler-Grafik-MC                           |   YES    |   Zilog 80    |  Z88DK  |      OK         |    missing     |   N/A ? |
*homelab*           | Homelab 4                                   |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A       |   N/A ? |
*homelab2*          | Homelab 2                                   |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A       |   N/A ? |
*kaypro83*          | Kaypro 83                                   |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A       |   N/A ? |
*kaypro*            | Kaypro 2X                                   |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A ?     |   N/A ? |
*kc*                | Robotron KC-85/2/3/4                        |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   N/A ? |
*kc85*              | Kyocera 85                                  |   YES    |  Intel 8085   |  Z88DK  |      OK         |    missing     | missing |
*kramermc*          | Kramer-MC                                   |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A       |   N/A ? |
*krokha*            | Krokha                                      |   YES?   |   Zilog 80    |  Z88DK  |      OK         |        ?       |    ?    |
*lambda*            | Lambda 8300                                 |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A       |   N/A ? |
*laser500*          | Laser 500                                   |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    |
*lm80c*             | LM80C                                       |   YES?   |   Zilog 80    |  Z88DK  |      OK?        |       OK       | missing |
*m10*               | Olivetti M10                                |   YES    |  Intel 8085   |  Z88DK  |      OK         |    missing     | missing |
*m100*              | TRS-80 M100                                 |   YES    |  Intel 8085   |  Z88DK  |      OK         |    missing     | missing |
*m5*                | Sord M5                                     |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK ?  |
*mbc200*            | Sanyo MBC 200                               |   YES?   |   Zilog 80    |  Z88DK  |      OK         |     missing    | missing |
*micro8085*         | Micro 8085                                  |   YES?   |  Intel 8085   |  Z88DK  |     N/A?        |      N/A?      |  N/A?   |
*mega65*            | Mega 65 / Commodore 65                      |    ?     |   MOS 6502    |   VBCC  |    missing      |    missing     | missing |
*mc10*              | TRS-80 MC-10 / Alice Matra 4k               |   YES    | Motorola 6803 | CC6303  |      OK         |    missing     |   OK    |
*mc1000*            | CCE MC-1000                                 |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    |
*microboard*        | Microboard Computer Development System      |   YES    |   RCA 1802    | LCC1802 |      OK         |       OK       |   OK    |
*microbee*          | Microobee                                   |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    |
*mikro80*           | Mikro 80                                    |   YES    |  Intel 8080   |  Z88DK  |      OK         |      N/A       |   N/A   |
*mo5*               | Thomson Mo series / Olivetti Prodest PC 128 |   YES    | Motorola 6809 |   CMOC  |      OK         |       OK       |   OK    |
*msx*               | MSX 1/MSX 2/2+/Turbo                        |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    |
*mtx*               | Memotech MTX 512                            |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK ?  |
*mtx500*            | Memotech MTX 500                            |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK ?  |
*multi8*            | Mitsubishi Multi 8                          |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    |
*myvision*          | Nichibutsu My Vision                        |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    |
*mz*                | Sharp MZ 80/700                             |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A       |   OK    |
*mz2500*            | Sharp MZ 2500                               |   YES    |   Zilog 80    |  Z88DK  |      OK         |    missing     | missing |
*nabu*              | Nabu PC                                     |   YES?   |   Zilog 80    |  Z88DK  |      OK?        |       OK?      | missing |
*nascom*            | Nascom                                      |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A       | missing |
*nc100*             | Amstrad NC-100/150                          |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   N/A ? |
*nc200*             | Amstrad NC-200                              |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   N/A ? |
*nes*               | Nintendo Enterteinment System               |   YES    |   MOS 6502    |   CC65  |      OK         |       OK       | missing |
*newbrain*          | Grundy Newbrain                             |   YES    |   Zilog 80    |  Z88DK  |    missing      |    missing     | missing |    
*ondra*             | Tesla Ondra Vili                            |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK ?     | missing |
*oric*              | Tangerine Oric 1/Atmos                      |   YES    |   MOS 6502    |   CC65  |      OK         |       OK       |   OK    |   
*osborne1*          | Osborne 1                                   |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A ?     |   N/A ? |
*osca*              | OSCA v6z80p                                 |   YES    |   Zilog 80    |  Z88DK  |      OK         |    missing     | missing |
*osic1p*            | Ohio Scientific Challenger 1P               |   YES    |   MOS 6502    |   CC65  |      OK         |      N/A ?     |   N/A ? |
*oz*                | Sharp OZ-700                                |   YES    |   Zilog 80    |  Z88DK  |     N/A ?       |      N/A ?     |   N/A ? |
*p2000*             | Philips P2000                               |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A       |   OK    |
*pacman*            | Pacman Namco arcade board                   |    NO    |   Zilog 80    |  Z88DK  |    missing      |    missing     | missing |
*pasaopia7*         | Toshiba Pasopia 7                           |   YES    |   Zilog 80    |  Z88DK  |      OK         |    missing     | missing | PSG SN76489 
*pc6001*            | NEC PC 6001                                 |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    |    
*pc8201*             | NEC PC 8201                                |   YES    |  Intel 8085   |  Z88DK  |      OK         |    missing     | missing |
*pc88*              | NEC PC 8801                                 |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK ?     | missing |
*pce*               | PCEngine                                    |   YES    |   MOS 6502    |   CC65  |      OK         |       OK       |   OK    |
*pcw*               | Amstrad PCW                                 |    ?     |   Zilog 80    |  Z88DK  |       ?         |    missing     | missing | 
*pecom*             | Pecom 32/64                                 |   YES    |   RCA 1802    | LCC1802 |      OK         |       OK       |   OK    |
*pencil2*           | Hanimex Pencil II                           |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK ?     | missing | PSG SN76489
*pet*               | Commodore Pet                               |   YES    |   MOS 6502    |   CC65  |      OK         |      N/A       |   OK    |
*pmd85*             | PMD85                                       |   YES    |  Intel 8080   |  Z88DK  |      OK         |       OK ?     | missing |
*pps*               | Peters Plus Sprinter                        |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A ?     | missing |
*primo*             | Microkey Primo                              |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK ?     |   OK ?  |
*pv1000*            | Casio PV1000                                |   YES    |   Zilog 80    |  Z88DK  |      OK         |    missing     | missing |
*pv2000*            | Casio PV2000                                |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       | missing | PSG SN76489
*px4*               | Epson PX-4/HC-40                            |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   N/A ? |
*px8*               | Epson PX-8/HC-80                            |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   N/A ? |
*qc10*              | Epson QX-10/QC-10                           |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A ?     |   N/A ? |
*radio86*           | Radio 86                                    |   YES?   |   Zilog 80    |  Z88DK  |      OK         |       OK?      | missing |
*rc700*             | Regnecentralen RC700                        |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A ?     |   N/A ? |
*rx78*              | Bandai RX-78                                |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       | missing | PSG SN76489
*samcoupe*          | Sam Coupe                                   |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    |     
*sc3000*            | Sega SC 3000                                |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    |         
*sg1000*            | Sega SG 1000                                |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    | 
*smc777*            | Sony SMC-70 / SMC-777                       |   YES    |   Zilog 80    |  Z88DK  |    missing      |    missing     | missing | PSG SN76489
*sms*               | Sega Master System                          |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK?   | PSG SN76489 Mednafen OK 
*sol20*             | Sol 20                                      |   YES?   |  Intel 8085   |  Z88DK  |      OK?        |    missing     | missing |
*sos*               | S-OS                                        |   YES    |   Zilog 80    |  Z88DK  |     N/A         |      N/A       |   N/A   |
*spc1000*           | Samsung SPC 1000                            |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    | 
*special*           | Specialist                                  |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A ?     |   N/A ? |
*spectrum*          | Sinclair Spectrum 16k/48k/128k              |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    | 
*srr*               | Exidy Sorcerer                              |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   N/A   |
*super80*           | Dick Smith Super 80/80d/80e/80m             |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A       |   OK    |
*super80r*          | Dick Smith Super 80r/80v                    |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK ?     |   OK    |
*supervision*       | Watara Supervision                          |   YES    |   MOS 6502    |   CC65  |      OK         |       OK       |   OK    |
*sv8000*            | Bandai Supervision 8000                     |   YES    |   Zilog 80    |  Z88DK  |      OK         |    missing     |   OK ?  | 4k rom limit
*svi*               | Spectravideo 318/328                        |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    | 
*T200*              | TRS-80 T200                                 |   YES    |  Intel 8085   |  Z88DK  |      OK         |    missing     | missing |
*telestrat*         | Oric Telestrat                              |   YES    |   MOS 6502    |   CC65  |    missing      |    missing     | missing |
*tmc600*            | Telmac TMC-600                              |   YES    |   RCA 1802    | LCC1802 |      OK         |      N/A       |   OK    | 
*ti82*              | Texas Instruments TI 82                     |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   N/A   | 
*ti83*              | Texas Instruments TI 83                     |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   N/A   | 
*ti8x*              | Texas Instruments TI 83 Plus                |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   N/A   | 
*ti85*              | Texas Instruments TI 85                     |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   N/A   | 
*ti86*              | Texas Instruments TI 86                     |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   N/A   | 
*tiki100*           | TIKI 100                                    |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    | 
*tim 011*           | Tim 011                                     |   YES?   |   Zilog 180   |  Z88DK  |      OK?        |       OK?      | missing |
*to7*               | Thomson To series                           |   YES    | Motorola 6809 |   CMOC  |      OK         |       OK       |   OK    | 
*trs80*             | TRS-80 Model I/III/IV                       |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A       |   OK    |
*ts2068*            | Timex TS 2068                               |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    | 
*tvc*               | Videoton TV Computer                        |   YES    |   Zilog 80    |  Z88DK  |    missing      |    missing     | missing |     
*vector06c*         | Вектор-06Ц Vector06c                        |   YES    |   Zilog 80    |  Z88DK  |      OK         |    missing     | missing |
*vg5k*              | VG-5000                                     |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    |
*gl4000*            | VTech Genius Leader 4000                    |   YES    |   Zilog 80    |  Z88DK  |    missing      |    missing     | missing |
*gl6000sl*          | VTech Genius Leader 6000sl                  |   YES    |   Zilog 80    |  Z88DK  |    missing      |    missing     | missing |
*v1050*             | V1050                                       |   YES?   |   Zilog 80    |  Z88DK  |      OK?        |       OK       | missing |
*vic20*             | Commodore Vic 20                            |   YES    |   MOS 6502    |   CC65  |      OK         |       OK       |   OK    | 
*vz200*             | VTech VZ200                                 |   YES    |   Zilog 80    |  Z88DK  |      OK         |    missing     |   OK    |
*x1*                | Sharp X1                                    |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK ?     |   OK    | mode 0 has udg
*x820*              | Xerox 820                                   |   YES?   |   Zilog 80    |  Z88DK  |      OK         |       OK ?     |   OK    |
*z1013*             | Robotron Z1013                              |   YES    |   Zilog 80    |  Z88DK  |      OK         |      N/A       |  N/A ?  |
*z80tvgame*         | ZX80 TV Game                                |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK ?     | missing |
*z88*               | Cambridge Z88                               |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |   OK    |
*z9001*             | Robotron Z9001, KC 85/1, KC 87              |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK ?     | missing |
*zx80*              | Sinclair ZX80                               |   YES    |   Zilog 80    |  Z88DK  |      OK ?       |    missing     |  N/A    |
*zx81*              | Sinclair ZX81                               |   YES    |   Zilog 80    |  Z88DK  |      OK         |       OK       |  N/A    |
*zxn*               | ZX Spectrum Next                            |   YES    |   Zilog 80    |  Z88DK  |      OK         |    missing     |   OK    |


TOTAL    : 166

OK       : 162

KO       :   2

UNKNOWN  :   2


-----------------------------------------------

#### Statistics on 8-bit targets with known existing emulators

Dev-kit | no. 8-bit targets
--------|--------------------
  Z88DK | 127
   CC65 |  22
LCC1802 |   7 
   CMOC |   5
   VBCC |   3
 CC6303 |   1


-----------------------------------------------

CPU family    | no. 8-bit targets  |
--------------|--------------------|
Zilog 80      | 115                |
MOS 6502      |  24                |
Intel 8080    |  12                |
RCA 1802      |   7                |
Motorola 6803 |   1                |
Motorola 6809 |   5                |
GBZ80         |   1                |


-----------------------------------------------------------------------------------------
### 8-BIT vintage targets with no known working emulators


target name         | long target name                                     |  working   | CPU family    |      dev-kit    |real-time input | hires graphics |  sound  | notes 
--------------------|------------------------------------------------------|------------|---------------|-----------------|----------------|----------------|---------|--------
*rex*               | Xircom Rex 6000                                      |     ?      |   Zilog 80    |          Z88DK  |    missing     |    missing     | missing | 








-----------------------------------------------------------------------------------------
### 8-BIT "new-retro" targets with no known emulators


target name         | long target name                                     |  working   | real-time input | hires graphics |  sound  | notes 
--------------------|------------------------------------------------------|------------|-----------------|----------------|---------|----------- 
*hbios*             | HBIOS generic target                                 |     ?      |    missing      |    missing     | missing |
*scz180*            | SC126 and SC130 boards                               |     ?      |    missing      |    missing     | missing | 
*rc2014*            | RC 2014 board                                        |     ?      |    missing      |    missing     | missing | 
*rcmx00*            | Rabbit Control Module                                |     ?      |    missing      |    missing     | missing | 
*yaz180*            | YAZ 180 board                                        |     ?      |    missing      |    missing     | missing | 


-----------------------------------------------------------------------------------------

### 16-BIT targets

target name         | long target name                                     |   working    | real-time input | hires graphics |  sound  | notes
--------------------|------------------------------------------------------|--------------|-----------------|----------------|---------|-----------
*amiga*             | Commodore Amiga                                      |    YES       |    missing      |    missing     | missing |
*atari_st*          | Atari ST                                             |    YES       |    missing      |    missing     | missing |
*olivetti_m20*      | Olivetti M20                                         |    YES       |    missing      |    missing     | missing |
*pc8086*            | PC 8088/86                                           |    YES       |    missing      |    missing     | missing |
*pdp11v7*           | PDP 11 V7                                            |     ?        |    missing      |    missing     | missing |
*sinclair_ql*       | Sinclair QL                                          |    YES       |    missing      |    missing     | missing |
*ti99*              | Texas Instruments TI99/4A                            |    YES       |      OK         |      OK        |   OK    |


OK       :   5

UNKNOWN  :   2


-----------------------------------------------------------------------------------------

### 32-BIT targets

target name         | long target name                                     |   working    | real-time input | hires graphics |  sound  | notes 
--------------------|------------------------------------------------------|--------------|-----------------|----------------|---------|----------- 
*linux386*          | ELF linux for Intel 386                              |    YES       |    missing      |    missing     | missing |
*linux68k*          | ELF linux for Motorola 68020                         |    YES       |    missing      |    missing     | missing | 
*linuxppc*          | ELF linux for PPC                                    |    YES       |    missing      |    missing     | missing |
*linuxmips*         | ELF linux for little endian MIPS32r2                 |     ?        |    missing      |    missing     | missing | 


OK       :   3

UNKNOWN  :   1

-----------------------------------------------------------------------------------------

### 32-BIT and 64-BIT targets on modern systems

target name         | long target name                                     |   working    | real-time input | hires graphics |  sound  | notes  
--------------------|------------------------------------------------------|--------------|-----------------|----------------|---------|----------- 
*gcc*               | Native Console version (with or without ncurses)     |    YES       |    missing      |    missing     | missing | 
*emcc*              | Emscripten (WebAssembly version)                     |    YES       |    missing      |    missing     | missing | input


OK       :   2

-----------------------------------------------------------------------------------------


### GLOBAL STATISTICS

TOTAL    : 185

OK       : 172

KO       :   4

UNKNOWN  :   9


