import argparse
import binascii
import array

# command line parsing
parser = argparse.ArgumentParser(description='Converts C64 PRG to a CRT file.')
parser.add_argument('input.prg', action = 'store')
parser.add_argument('output.crt', action = 'store')
args = parser.parse_args()
inputFilename = args.__dict__['input.prg']
outputFilename = args.__dict__['output.crt']

# open PRG file and read into array
inputFile = open(inputFilename, 'rb')
prg = bytearray(inputFile.read())

# output data array
crtData = bytearray()

# helper function to write binary data to the CRT file
def writeHex(string):
	global crtData
	crtData += bytearray.fromhex(string)

# CRT signature
writeHex('43 36 34 20 43 41 52 54 52 49 44 47 45 20 20 20')

# header length
writeHex('00 00 00 40')

# cartridge version
writeHex('01 00')

# cartridge hardware type: Magic Desk
writeHex('00 13')

# EXROM status
writeHex('00')

# GAME status
writeHex('01')

# reserved
writeHex('00 00 00 00 00 00')

# cartridge name
writeHex('00'*32)


# ROM content
content = bytearray()

# add loader (from "loader.bin" file)
#loaderFile = open('loader.bin', 'rb')
#loader = loaderFile.read()
#content += bytearray(loader)
#print ''.join( [ "%02X " % ord( x ) for x in loader ] ).strip()

# add loader inline
content += bytearray.fromhex(
	'09 80 09 80 C3 C2 CD 38 30 78 8E 16 D0 20 A3 FD 20 50 FD A9 A0 8D 84 02 20 15' +
	'FD 20 5B FF 58 20 53 E4 20 BF E3 20 22 E4 A2 FB 9A A9 00 85 57 AD 9A 80 85 58' +
	'AD 9B 80 85 59 AD 9C 80 85 2D AD 9D 80 85 2E A9 9E 85 5A A9 80 85 5B A2 00 BD' +
	'59 80 9D 00 06 E8 D0 F7 4C 00 06 78 A2 00 A5 57 8D 00 DE A1 5A 81 2D E6 5A D0' +
	'13 E6 5B A5 5B C9 A0 D0 0B A9 80 85 5B E6 57 A5 57 8D 00 DE E6 2D D0 02 E6 2E' +
	'C6 58 D0 DB C6 59 A5 59 C9 FF D0 D3 A9 80 8D 00 DE 20 63 A6 58 4C AE A7')

# add program size, minus the first two bytes for start address
size = len(prg) - 2
content.append(size & 0xff)
content.append(size / 0x100)

# add program, with start address
content += prg

# align to 0x2000 bytes
while (len(content) & 0x1fff) != 0:
	content.append(0)


# save content as Chip blocks
banks = len(content) / 0x2000
for bank in range(0, banks):
	# CHIP
	writeHex('43484950')
	
	# total packet length: 0x2010 (ROM image size + CHIP header)
	writeHex('00 00 20 10')
	
	# chip type: ROM
	writeHex('00 00')
	
	# bank number
	crtData.append(0)
	crtData.append(bank)
	
	# starting load address
	writeHex('80 00')
	
	# ROM image size
	writeHex('20 00')
	
	# 0x2000 bytes ROM image
	start = 0x2000 * bank
	end = start + 0x2000
	crtData += content[start:end]

# save module
crt = open(outputFilename, 'wb')
crt.write(crtData)
crt.close()
