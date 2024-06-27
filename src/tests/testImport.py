import unittest
import sys

# sys.path.append("../assets/")
# from import_from_source import *
sys.path.append("assets")

# from import_from_source import *

from import_from_source import *

class testImport(unittest.TestCase):

    # fill_empty_slots_with_zero
    def test_fill_empty_slots_with_zero_1(self):
        filled_slots = fill_empty_slots_with_zero(["","1","2","", "3","",""])
        self.assertEqual(filled_slots,['0', '1', '2', '0', '3', '0', '0'])


    # has_headless_hex
    def test_has_headless_hex_1(self):
        has_headless_hex_check = has_headless_hex(["AB,CD,EF,01,23,45,67,89","AF"])
        self.assertTrue(has_headless_hex_check)

    def test_has_headless_hex_2(self):
        has_headless_hex_check = has_headless_hex(["ABC,CD,EF,01,23,45,67,89","AF"])
        self.assertFalse(has_headless_hex_check)

    def test_has_headless_hex_2(self):
        has_headless_hex_check = has_headless_hex(["AB,$CD,EF,01,23,45,67,89","AF"])
        self.assertFalse(has_headless_hex_check)


    # has_nine_byte_lines
    def test_has_nine_byte_lines_1(self):
        has_headless_hex_check = has_nine_byte_lines(["AB,CD,EF,01,23,45,67,89,AF"])
        self.assertTrue(has_headless_hex_check)

    def test_has_nine_byte_lines_2(self):
        has_headless_hex_check = has_nine_byte_lines(["AB,CD,EF,01,23,45,67,89"])
        self.assertFalse(has_headless_hex_check)

    def test_has_nine_byte_lines_3(self):
        has_headless_hex_check = has_nine_byte_lines(["A,CD,EF,1,23,45,67,89,AF"])
        self.assertTrue(has_headless_hex_check)


    # remove_basic_comments
    def test_remove_basic_comments_1(self):
        cleaned_string = remove_basic_comments('10 PRINT 42:REM ABC')
        self.assertEqual(cleaned_string,"10 PRINT 42")

    def test_remove_basic_comments_2(self):
        cleaned_string = remove_basic_comments('10 ? 42:rem ABC')
        self.assertEqual(cleaned_string,"10 ? 42")

    def test_remove_basic_comments_3(self):
        cleaned_string = remove_basic_comments('10 PRINT 42: REM DEF')
        self.assertEqual(cleaned_string,"10 PRINT 42")

    def test_remove_basic_comments_4(self):
        cleaned_string = remove_basic_comments('10 ? 42: rem DEF')
        self.assertEqual(cleaned_string,"10 ? 42")

    def test_remove_basic_comments_5(self):
        cleaned_string = remove_basic_comments("10 ? 42:' DEF")
        self.assertEqual(cleaned_string,"10 ? 42")

    def test_remove_basic_comments_6(self):
        cleaned_string = remove_basic_comments("10 ? 42: ' DEF")
        self.assertEqual(cleaned_string,"10 ? 42")


    # remove_assembly_comments
    def test_remove_assembly_comments_1(self):
        cleaned_string = remove_assembly_comments("!BYTE $FF ; FOO")
        self.assertEqual(cleaned_string,"!BYTE $FF")


    # normalize_assembly_line
    def test_normalize_assembly_line_1(self):
        normalized_line = normalize_assembly_line("0x2A")
        self.assertEqual(normalized_line,"$2A")
        
    def test_normalize_assembly_line_2(self):
        normalized_line = normalize_assembly_line("#2A")
        self.assertEqual(normalized_line,"$2A")

    def test_normalize_assembly_line_3(self):
        normalized_line = normalize_assembly_line(">2A")
        self.assertEqual(normalized_line,"$2A")

    def test_normalize_assembly_line_4(self):
        normalized_line = normalize_assembly_line("&H2A &h2B")
        self.assertEqual(normalized_line,"$2A $2B")

    def test_normalize_assembly_line_5(self):
        normalized_line = normalize_assembly_line("#2A")
        self.assertEqual(normalized_line,"$2A")

    def test_normalize_assembly_line_6(self):
        normalized_line = normalize_assembly_line("0b01010101")
        self.assertEqual(normalized_line,"@01010101")

    def test_normalize_assembly_line_7(self):
        normalized_line = normalize_assembly_line("%10101010")
        self.assertEqual(normalized_line,"@10101010")


    # normalize_basic_line
    def test_normalize_basic_line_1(self):
        normalized_line = normalize_basic_line("20 SYMBOL AFTER 32")
        self.assertEqual(normalized_line,"")

    def test_normalize_basic_line_2(self):
        normalized_line = normalize_basic_line("20 SYMBOL 32")
        self.assertEqual(normalized_line,"20 SYMBOL 32")

    def test_normalize_basic_line_3(self):
        normalized_line = normalize_basic_line('30 POKE USR "a"+3,BIN 0111110')
        self.assertEqual(normalized_line,'30 POKE USR "a"+3,\nBIN@ 0111110')

    # compute_shape
    def test_compute_shape_xsize_8_1(self):
        csv_string = "255,255,255,255,255,255,255,255"
        self.assertEqual(compute_shape(csv_string, 8),["########","########","########","########","########","########","########","########",])

    def test_compute_shape_xsize_8_2(self):
        csv_string = "0,0,0,0,0,0,0,0"
        self.assertEqual(compute_shape(csv_string, 8),["........","........","........","........","........","........","........","........",])

    def test_compute_shape_xsize_8_3(self):        
        csv_string = "1,2,3,4,5,6,7,8"
        self.assertEqual(compute_shape(csv_string, 8),[".......#","......#.","......##",".....#..",".....#.#",".....##.",".....###","....#...",])

    def test_compute_shape_xsize_8_4(self):       
        csv_string = "1,  2,3,4,  5,6  ,7,8"
        self.assertEqual(compute_shape(csv_string, 8),[".......#","......#.","......##",".....#..",".....#.#",".....##.",".....###","....#...",])       

    def test_compute_shape_xsize_8_5(self):        
        csv_string = "254,253,252,251,250,249,248,247"
        self.assertEqual(compute_shape(csv_string, 8),["#######.","######.#","######..","#####.##","#####.#.","#####..#","#####...","####.###",])   

    def test_compute_shape_xsize_7_1(self):
        csv_string = "127,127,127,127,127,127,127,127"
        self.assertEqual(compute_shape(csv_string, 7),["#######","#######","#######","#######","#######","#######","#######","#######",])

    def test_compute_shape_xsize_7_2(self):
        csv_string = "0,0,0,0,0,0,0,0"
        self.assertEqual(compute_shape(csv_string, 7),[".......",".......",".......",".......",".......",".......",".......",".......",])

    def test_compute_shape_xsize_7_3(self):        
        csv_string = "1,2,3,4,5,6,7,8"
        self.assertEqual(compute_shape(csv_string, 7),["......#",".....#.",".....##","....#..","....#.#","....##.","....###","...#...",])

    def test_compute_shape_xsize_7_4(self):       
        csv_string = "1,  2,3,4,  5,6  ,7,8"
        self.assertEqual(compute_shape(csv_string, 7),["......#",".....#.",".....##","....#..","....#.#","....##.","....###","...#...",])       

    def test_compute_shape_xsize_7_5(self):        
        csv_string = "126,125,124,123,122,121,120,119"
        self.assertEqual(compute_shape(csv_string, 7),["######.","#####.#","#####..","####.##","####.#.","####..#","####...","###.###",])   

    def test_compute_shape_xsize_6_1(self):
        csv_string = "63,63,63,63,63,63,63,63"
        self.assertEqual(compute_shape(csv_string, 6),["######","######","######","######","######","######","######","######",])

    def test_compute_shape_xsize_6_2(self):
        csv_string = "0,0,0,0,0,0,0,0"
        self.assertEqual(compute_shape(csv_string, 6),["......","......","......","......","......","......","......","......",])

    def test_compute_shape_xsize_6_3(self):        
        csv_string = "1,2,3,4,5,6,7,8"
        self.assertEqual(compute_shape(csv_string, 6),[".....#","....#.","....##","...#..","...#.#","...##.","...###","..#...",])

    def test_compute_shape_xsize_6_4(self):       
        csv_string = "1,  2,3,4,  5,6  ,7,8"
        self.assertEqual(compute_shape(csv_string, 6),[".....#","....#.","....##","...#..","...#.#","...##.","...###","..#...",])       

    def test_compute_shape_xsize_6_5(self):        
        csv_string = "62,61,60,59,58,57,56,55"
        self.assertEqual(compute_shape(csv_string, 6),["#####.","####.#","####..","###.##","###.#.","###..#","###...","##.###",])   


    # compute_rotated_shape
    @staticmethod
    def intListToString(intList):
        res = ""
        for value in intList:
            res+=str(value)+","
        res=res[:-1]
        return res  


    # compute_rotated_shape
    def test_compute_rotated_shape_1(self):
       
        initial_list = [1,2,3,4,5,6,7,8]
            # .......#
            # ......#.
            # ......##
            # .....#..
            # .....#.#
            # .....##.
            # .....###
            #....#...
        
        initial_list_string = testImport.intListToString(initial_list)
        
        shape0 = compute_shape(initial_list_string,8)
        expected_result = [0, 0, 0, 0, 128, 120, 102, 85]   
        self.assertEqual(compute_rotated_shape(shape0),expected_result)
        expected_result_string = testImport.intListToString(expected_result)

        shape1 = compute_shape(expected_result_string,8)
        expected_result = [16, 224, 96, 160, 32, 192, 64, 128]
        self.assertEqual(compute_rotated_shape(shape1), expected_result)
        expected_result_string = testImport.intListToString(expected_result)       
        
        shape2 = compute_shape(expected_result_string,8)
        expected_result = [170, 102, 30, 1, 0, 0, 0, 0]
        self.assertEqual(compute_rotated_shape(shape2), expected_result)
        expected_result_string = testImport.intListToString(expected_result)
        
        shape3 = compute_shape(expected_result_string,8)
        expected_result = initial_list 
        self.assertEqual(compute_rotated_shape(shape3), expected_result)  
        expected_result_string = testImport.intListToString(expected_result)

    def test_compute_rotated_shape_2(self):
       
        initial_list = [255,0,255,0,255,0,255,0]
        
        initial_list_string = testImport.intListToString(initial_list)
        
        shape0 = compute_shape(initial_list_string,8)
        expected_result = [85, 85, 85, 85, 85, 85, 85, 85]   
        self.assertEqual(compute_rotated_shape(shape0),expected_result)
        expected_result_string = testImport.intListToString(expected_result)

        shape1 = compute_shape(expected_result_string,8)
        expected_result = [0,255,0,255,0,255,0,255]
        self.assertEqual(compute_rotated_shape(shape1), expected_result)
        expected_result_string = testImport.intListToString(expected_result)       
        
        shape2 = compute_shape(expected_result_string,8)
        expected_result = [170, 170, 170, 170, 170, 170, 170, 170]
        self.assertEqual(compute_rotated_shape(shape2), expected_result)
        expected_result_string = testImport.intListToString(expected_result)
        
        shape3 = compute_shape(expected_result_string,8)
        expected_result = initial_list 
        self.assertEqual(compute_rotated_shape(shape3), expected_result)  
        expected_result_string = testImport.intListToString(expected_result)


    # aux_rip_tiles
    def test_rip_tiles_asm_1(self):
        source_lines = ['!byte 0,0,60,98,126,98,98,98\n']
        ripped_lines = aux_rip_tiles(source_lines,False,False,8,8,False,False)
        
        self.assertEqual(ripped_lines,['0,0,60,98,126,98,98,98'])

    def test_rip_tiles_asm_2(self):
        source_lines = ['!byte 0x00, 0x1F, 0x28, 0x28, 0x28, 0x3F, 0x1F, 0x00\n']
        ripped_lines = aux_rip_tiles(source_lines,False,False,8,8,False,False)
        
        self.assertEqual(ripped_lines,['$00,$1F,$28,$28,$28,$3F,$1F,$00'])
        
    def test_rip_tiles_asm_3(self):
        rip_flag = True
        self.maxDiff = None
        source_lines = [ \
            '; CHARSET IMAGE DATA...',
            '; 48 images, 8 bytes per image, total size is 384 ($180) bytes.',
            '\n',
            'charset_data',
            '\n',
            '.byte $00,$00,$00,$00,$00,$00,$00,$00,$7C,$EE,$C6,$C6,$FE,$C6,$C6,$00',
            '.byte $FC,$C6,$C6,$FC,$C6,$C6,$FE,$00,$7E,$E6,$C0,$C0,$C0,$E6,$7E,$00',
            '.byte $FC,$CE,$C6,$C6,$C6,$CE,$FC,$00,$FE,$C6,$C0,$F8,$C0,$C6,$FE,$00',
            '.byte $FE,$C6,$C0,$F8,$C0,$C0,$C0,$00,$7E,$E6,$C0,$CE,$C6,$E6,$7E,$00',
            '.byte $C6,$C6,$C6,$C6,$FE,$C6,$C6,$00,$3C,$18,$18,$18,$18,$18,$3C,$00',
            '.byte $06,$06,$06,$06,$C6,$EE,$7C,$00,$CE,$DC,$F8,$F8,$DC,$CE,$C6,$00',
            '.byte $C0,$C0,$C0,$C0,$C0,$C6,$FE,$00,$C6,$EE,$FE,$FE,$C6,$C6,$C6,$00',
            '.byte $C6,$E6,$F6,$FE,$DE,$CE,$C6,$00,$FC,$CE,$C6,$C6,$C6,$E6,$7E,$00',
            '.byte $FC,$CE,$C6,$C6,$FE,$C0,$C0,$00,$FC,$CE,$C6,$C6,$E6,$7C,$0E,$00',
            '.byte $FC,$CE,$C6,$C6,$FE,$DC,$CE,$00,$FE,$C6,$C0,$7C,$06,$C6,$FE,$00',
            '.byte $FE,$36,$30,$30,$30,$30,$30,$00,$C6,$C6,$C6,$C6,$C6,$EE,$7C,$00',
            '.byte $C6,$C6,$C6,$EE,$7C,$38,$10,$00,$C6,$C6,$C6,$C6,$FE,$EE,$C6,$00',
            '.byte $C6,$6C,$38,$38,$6C,$C6,$C6,$00,$C6,$EC,$78,$30,$30,$30,$30,$00',
            '.byte $FE,$CE,$1C,$38,$70,$E6,$FE,$00,$7C,$EE,$C6,$C6,$C6,$EE,$7C,$00',
            '.byte $38,$18,$18,$18,$18,$18,$18,$00,$FC,$C6,$06,$1C,$70,$E0,$FE,$00',
            '.byte $FC,$C6,$06,$3C,$06,$C6,$FC,$00,$C0,$C0,$C0,$D8,$FE,$18,$18,$00',
            '.byte $FE,$C6,$C0,$FE,$06,$C6,$7C,$00,$1C,$38,$70,$EC,$C6,$E6,$7E,$00',
            '.byte $FE,$C6,$0C,$18,$30,$30,$30,$00,$7C,$C6,$C6,$7C,$C6,$C6,$7C,$00',
            '.byte $7E,$67,$63,$37,$0E,$1C,$38,$00,$00,$00,$00,$00,$00,$30,$30,$00',
            '.byte $00,$00,$00,$00,$00,$18,$30,$00,$7E,$66,$06,$1C,$18,$00,$18,$00',
            '.byte $18,$18,$18,$18,$18,$00,$18,$00,$18,$18,$18,$00,$00,$00,$00,$00',
            '.byte $6C,$6C,$6C,$00,$00,$00,$00,$00,$18,$30,$30,$30,$30,$30,$18,$00',
            '.byte $18,$0C,$0C,$0C,$0C,$0C,$18,$00,$00,$00,$FC,$7E,$00,$00,$00,$00',
            '.byte $00,$18,$18,$7E,$18,$18,$00,$00,$00,$00,$18,$00,$00,$18,$00,$00',
            '\n',
            '\n',
            '; CHARSET IMAGE ATTRIBUTE DATA...',
            '; 48 attributes, 1 attribute per image, 8 bits per attribute, total size is 48 ($30) bytes.',
            '; nb. Upper nybbles = material, lower nybbles = colour.',
            '\n',
            'charset_attrib_data',
            '\n',
            '.byte $03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03',
            '.byte $03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03',
            '.byte $03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03',
            ]
        ripped_lines = aux_rip_tiles(source_lines,False,False,8,8,rip_flag,False)
        
        self.assertEqual(ripped_lines, \
            [ \
            '$00,$00,$00,$00,$00,$00,$00,$00',
            '$7C,$EE,$C6,$C6,$FE,$C6,$C6,$00',
            '$FC,$C6,$C6,$FC,$C6,$C6,$FE,$00',
            '$7E,$E6,$C0,$C0,$C0,$E6,$7E,$00',
            '$FC,$CE,$C6,$C6,$C6,$CE,$FC,$00',
            '$FE,$C6,$C0,$F8,$C0,$C6,$FE,$00',
            '$FE,$C6,$C0,$F8,$C0,$C0,$C0,$00',
            '$7E,$E6,$C0,$CE,$C6,$E6,$7E,$00',
            '$C6,$C6,$C6,$C6,$FE,$C6,$C6,$00',
            '$3C,$18,$18,$18,$18,$18,$3C,$00',
            '$06,$06,$06,$06,$C6,$EE,$7C,$00',
            '$CE,$DC,$F8,$F8,$DC,$CE,$C6,$00',
            '$C0,$C0,$C0,$C0,$C0,$C6,$FE,$00',
            '$C6,$EE,$FE,$FE,$C6,$C6,$C6,$00',
            '$C6,$E6,$F6,$FE,$DE,$CE,$C6,$00',
            '$FC,$CE,$C6,$C6,$C6,$E6,$7E,$00',
            '$FC,$CE,$C6,$C6,$FE,$C0,$C0,$00',
            '$FC,$CE,$C6,$C6,$E6,$7C,$0E,$00',
            '$FC,$CE,$C6,$C6,$FE,$DC,$CE,$00',
            '$FE,$C6,$C0,$7C,$06,$C6,$FE,$00',
            '$FE,$36,$30,$30,$30,$30,$30,$00',
            '$C6,$C6,$C6,$C6,$C6,$EE,$7C,$00',
            '$C6,$C6,$C6,$EE,$7C,$38,$10,$00',
            '$C6,$C6,$C6,$C6,$FE,$EE,$C6,$00',
            '$C6,$6C,$38,$38,$6C,$C6,$C6,$00',
            '$C6,$EC,$78,$30,$30,$30,$30,$00',
            '$FE,$CE,$1C,$38,$70,$E6,$FE,$00',
            '$7C,$EE,$C6,$C6,$C6,$EE,$7C,$00'
            ]
        )


    def test_rip_tiles_asm_4(self):
        rip_flag = True
        assembly_extension = True

        self.maxDiff = None
        source_lines = [ \
            "PAT0   DATA >1824,>1866,>BD3C,>2424"
            "PAT1   DATA >0000,>0000,>0000,>0000"
            ]
        ripped_lines = aux_rip_tiles(source_lines,assembly_extension,False,8,8,rip_flag,False)
        
        self.assertEqual(ripped_lines, \
            [ \
            '$18,$24,$18,$66,$BD,$3C,$24,$24',
            ]
        )
           


    def test_rip_tiles_asm_5(self):
        rip_flag = True
        assembly_extension = False

        self.maxDiff = None
        source_lines = [ \
            '.byte %01010101',
            '.byte %11111111',
            '.byte %10101010',
            '.byte %00110011',
            '.byte %11001100',
            '.byte %01010101',
            '.byte %10101010',
            '.byte %11111111',
            ]
        ripped_lines = aux_rip_tiles(source_lines,assembly_extension,False,8,8,rip_flag,False)
        
        self.assertEqual(ripped_lines, \
            [ \
            '85,255,170,51,204,85,170,255',
            ]
        )

    def test_rip_tiles_asm_6(self):
        rip_flag = True
        assembly_extension = False

        self.maxDiff = None
        source_lines = [ \
            '; Exported using VChar64 v0.2.4',
            '; Total bytes: 2048',
            'charset:',
            'foo .byte $3c,$66,$6e,$6e,$60,$62,$3c',
            '.byte $00,$18,$3c,$66,$7e,$66,$66,$66,$00       ; 0',
            'bar .byte $7c,$66,$66,$7c,$66,$66,$7c,$00,$3c,$66,$60,$60,$60,$66,$3c,$00       ; 16',
            'toto .byte $78,$6c,$66,$66,$66,$6c,$78,$00,$7e,$60,$60,$78,$60,$60,$7e,$00      ; 32',
            '.byte $7e,$60,$60,$78,$60,$60,$60,$00,$3c,$66,$60,$6e,$66,$66,$3c,$00   ; 48',
            '.byte $66,$66,$66,$7e,$66,$66,$66,$00,$3c,$18,$18,$18,$18,$18,$3c,$00   ; 64',
            '.byte $1e,$0c,$0c,$0c,$0c,$6c,$38,$00,$66,$6c,$78,$70,$78,$6c,$66,$00   ; 80',
            '.byte $60,$60,$60,$60,$60,$60,$7e,$00,$63,$77,$7f,$6b,$63,$63,$63,$00   ; 96',
            ]
        ripped_lines = aux_rip_tiles(source_lines,assembly_extension,False,8,8,rip_flag,False)
        
        self.assertEqual(ripped_lines, \
            [ \
            '$3c,$66,$6e,$6e,$60,$62,$3c,$00',
            '$18,$3c,$66,$7e,$66,$66,$66,$00',
            '$7c,$66,$66,$7c,$66,$66,$7c,$00',
            '$3c,$66,$60,$60,$60,$66,$3c,$00',
            '$78,$6c,$66,$66,$66,$6c,$78,$00',
            '$7e,$60,$60,$78,$60,$60,$7e,$00',
            '$7e,$60,$60,$78,$60,$60,$60,$00',
            '$3c,$66,$60,$6e,$66,$66,$3c,$00',
            '$66,$66,$66,$7e,$66,$66,$66,$00',
            '$3c,$18,$18,$18,$18,$18,$3c,$00',
            '$1e,$0c,$0c,$0c,$0c,$6c,$38,$00',
            '$66,$6c,$78,$70,$78,$6c,$66,$00',
            '$60,$60,$60,$60,$60,$60,$7e,$00',
            '$63,$77,$7f,$6b,$63,$63,$63,$00'
            ]
        )


    def test_rip_tiles_cbm_basic_1(self):
        rip_flag = False
        source_lines = [ \
            '000 REM ** USER DEFINED GRAPHICS **',
            '001 REM ** BY SHANE JOLLY **',
            '100 PRINT CHR$(147): PRINT TAB(12)"PLEASE WAIT..."',
            '110 PRINT CHR$(142)',
            '120 POKE 52, 48: POKE 56, 48',
            '130 POKE UG=12288;CG=53248',
            '140 POKE 56334, PEEK(56334) AND 254',
            '150 POKE 1, PEEK(1) AND 251',
            '160 FOR K=1 TO 2047',
            '170 POKE UG+K, PEEK(CG+K)',
            '180 NEXT',
            '190 POKE 1, PEEK(1) OR 4',
            '200 POKE 56334, PEEK(6334) OR 1',
            '210 POKE 53272, (PEEK(53272) AND 240)+12',
            '220 FOR CC=1 TO 9',
            '230 FOR J=0 TO 7',
            '240 READ A: POKE UG+8*CC+J, A',
            '250 NEXT J',
            '260 NEXT CC',
            '270 FOR N=65 TO 90',
            '280 PRINT CHR$(N);" ";',
            '290 NEXT N',
            '300 DATA 000, 016, 040, 068, 124, 068, 068, 000',
            '310 DATA 000, 120, 072, 124, 068, 068, 124, 000',
            '320 DATA 000, 124, 068, 064, 064, 068, 124, 000',
            '330 DATA 000, 120, 068, 068, 068, 068, 120, 000',
            '340 DATA 000, 124, 068, 112, 112, 068, 124, 000',
            '350 DATA 000, 124, 068, 112, 064, 064, 064, 000',
            '360 DATA 000, 124, 068, 064, 064, 078, 124, 000',
            '370 DATA 000, 068, 068, 124, 124, 068, 068, 000',
            '380 DATA 000, 056, 016, 016, 016, 016, 056, 000',
            ]
        ripped_lines = aux_rip_tiles(source_lines,False,False,8,8,rip_flag,False)
        
        self.assertEqual(ripped_lines, \
        [ \
            '000,016,040,068,124,068,068,000',
            '000,120,072,124,068,068,124,000',
            '000,124,068,064,064,068,124,000',
            '000,120,068,068,068,068,120,000',
            '000,124,068,112,112,068,124,000',
            '000,124,068,112,064,064,064,000',
            '000,124,068,064,064,078,124,000',
            '000,068,068,124,124,068,068,000',
            '000,056,016,016,016,016,056,000' \
        ] \
        )

    def test_rip_tiles_cbm_basic_2(self):
        rip_flag = False
        source_lines = [ \
            '0l=32:v=36878:fOk=.to4:rEb$:fOi=1to8:pO7167+i+k*8,aS(mI(b$,i)):pO7421+i,.:nE:nE',
            '1pO52,26:pO56,26:n=1:t=30720:pOv-9,255:pOv+1,8:?"{clear}{reverse on}{yellow}ijkl":x=3:p=7955:ifj>rtHr=j',
            '2wA198,1:g(0)=211:g(1)=209:g(2)=215:g(3)=218:pO650,128:u=22:k=7745:y=7732:z=y-8',
            '3gEa$:b=z+rN(.)*464:q=rN(1)*4:pOb,g(q):pOb+t,q+2:ifj>=100*ntHx=x-(x<9):n=n+1',
            '4pOk+i,l:pOy+i*u,l:i=i+1:ifi<=utHpOk+i,4:pOk+i+t,6:pOy+i*u,4:pOy+i*u+t,7:m=pE(p)',
            '5g=.:x=x+(m=4):e=saN1:c=s-1+e:ifi>utHi=.:y=z+rN(.)*u-1:k=z+u*int((p-z)/u)-1',
            '6ifc=sG(c)tHg=1:h=s+1:pOp,l:p=p+c*21*e+c:d=pE(p):ifd=ftHw=w-(d>l):j=j-(w>1)*w',
            '7pOp+t,7:?"{home}{reverse on}{red}g{green}e{cyan}m{purple}s{white}"j" {blue}hi{cyan}"r" {yellow}{reverse off}b{green}{reverse on}"x:ifd>ltHifgtHpOv,9:pOv,.:dA"{24}{60}{24}{102}{153}{24}{36}{102}"',
            '8pOp,h:w=-w*(d=f):s=aS(a$+"@")-74:dA"{24}{44}{152}{110}{25}{24}{40}{40}","{24}{36}{24}{102}{153}{24}{36}{102}","{24}{52}{25}{118}{152}{24}{40}{40}","{ 24}{ 36}{ 90}{189}{189}{ 90}{ 36}{ 24}"',
            '9f=d:on-(x>.aNp<8186aNp>=z)gO3:?sP7)"{reverse on}{red}game over":fOa=.tov/9:nE:pO198,0:j=.:gO1',
            ]
        ripped_lines = aux_rip_tiles(source_lines,False,False,8,8,rip_flag,False)
        
        self.assertEqual(ripped_lines, \
        [ \
            '24,60,24,102,153,24,36,102',
            '24,44,152,110,25,24,40,40',
            '24,36,24,102,153,24,36,102',
            '24,52,25,118,152,24,40,40',
            '24,36,90,189,189,90,36,24' 
        ] \
        )

    def test_rip_tiles_cbm_basic_3(self):
        rip_flag = False
        source_lines = [ \
            '0pO56,26:pO52,26:v=36878:x=132:pOv-9,255:fOp=.to39:rEb:pO7168+p,b:pO7421+p,.:nE',
            '1e=22:z=218:o=4:y=3:b=7:p=8174:pO650,z:?"{clear}{black}{reverse on}jl":l$="{green}{68*2}{black}{66}":r$="{black}{67}{green}{68*2}":ifi>rtHr=i',
            '2n=v+1:t=30720:pOn,59:wA198,1:?"{clear}":fOi=3toe:?tA4)"{green}{68}{black}{reverse on}{230}{191}"sP8)"{191}{230}{reverse off}{green}{68}":nE:d=1:i=.',
            '3b=b+f:i=i+1:if(iaNy)=ytHd=int(rN(.)*3):ifb<4tHifo<6tHb=4:y=7:dA24,189,255,165',
            '4s=int(rN(.)*(b+1)):ifpE(p-e)<>32tHpOn,42:pOv,15:pOv,.:l=l+1:fOf=-9tox:nE:pOn,59',
            '5c$=" ":gEa$:ifiaN1tHc$="{31}@":ifl<.tHl=.:dA36,189,255,165,90,102,90,60,36,219,255',
            '6?"{home}{down}{left}{148}":pOz,x:pOp+t,2:pOp,1:ifpE(p-e)=4tHl=9:dA195,93,114,101,66,93,114,101,66',
            '7?sPo)l$sPs)c$sPb-s)r$:ifa$<>""tHk=aS(a$):p=p+(k=74)-(k=76):dA93,39,83,33,93,39',
            '8?"{home}{red}a{reverse on}"l" {purple}"i" {black}"r:f=(iaN127)=.:l=l+f:ifo>1-dtHifo+d+b<17tHo=o-1+d:dA83,33,8,28',
            '9on-(l<9)gO3:?"{home}{down*2}"sP8)"{black}{reverse on}{191} {red}end {black}{191}":fOs=ttov:nE:l=.:pO198,0:gO1:dA62,28,62,8,8,',
            ]
        ripped_lines = aux_rip_tiles(source_lines,False,False,8,8,rip_flag,False)
        
        self.assertEqual(ripped_lines, \
            [
            '24,189,255,165,36,189,255,165',
            '90,102,90,60,36,219,255,195',
            '93,114,101,66,93,114,101,66',
            '93,39,83,33,93,39,83,33',
            '8,28,62,28,62,8,8,0'
            ]

        )

    def test_rip_tiles_oric_basic_1(self):
        rip_flag = True
        source_lines = [ \
            '10 REM Golf',
            '20 CH=46080',
            '30 FOR Q=1 TO 3',
            '40 READ C',
            '50 FOR I=0 TO 7',
            '60 READ D',
            '70 POKE CH+C+I,D',
            '80 NEXT I',
            '90 NEXT Q',
            '100 DATA 264,8,12,14,8,8,8,8,62',
            '110 DATA 512,12,30,45,30,12,18,18,33',
            '120 DATA 280,12,30,31,46,62,60,28,24',
            '130 DIM T(9)',
            '140 XH =0:XC=0',
            '150 YH=0:HT=0:YC=0',
            '170 FOR H=1 TO 9',
            ]
        ripped_lines = aux_rip_tiles(source_lines,False,False,8,8,rip_flag,False)
        
        self.assertEqual(ripped_lines, \
            [ \
            '8,12,14,8,8,8,8,62', 
            '12,30,45,30,12,18,18,33', 
            '12,30,31,46,62,60,28,24'
            ]
        )

    def test_rip_tiles_msx_basic_1(self):
        rip_flag = True
        self.maxDiff = None
        source_lines = [ \
            "490 DATA 22,63,F7,B7,FF,7E,3C,3C ' a",
            "491 DATA 38,6C,FF,3F,0F,3F,FC,38 ' b",
            "492 DATA 3C,3C,7E,FF,ED,EF,C6,44 ' c",
            "493 DATA 1C,36,FF,FC,F0,FC,3F,1C ' d",
            "494 DATA 3C,7E,FF,FF,FF,FF,7E,3C ' e",
            "495 DATA 08,10,54,FE,FE,FE,FE,7C ' f",
            "496 DATA 06,08,10,10,38,7C,7C,38 ' g",
            "497 DATA 00,28,FE,AA,FE,54,38,10 ' h",
            "500 DATA 08,1C,2A,7F,77,3E,36,63 ' i",
            "501 DATA FF,D5,AB,D5,AB,D5,AB,FF ' j",
            "502 DATA 00,00,00,18,18,00,00,00 ' k",
            "503 DATA 00,00,18,24,24,18,00,00 ' l",
            "504 DATA 00,18,3C,66,66,3C,18,00 ' m",
            "505 DATA 3C,7E,E7,C3,C3,E7,7E,3C ' n",
            "510 DATA 00,01,06,1D,2A,2A,2A,1F ' o",
            "511 DATA 1B,8F,65,11,C9,A9,B1,F3 ' p",
            "512 DATA 4C,F7,F0,18,07,02,3E,FE ' q",
            "513 DATA 7F,9F,31,41,81,81,F9,FD ' r",
            "515 DATA 7E,C3,9F,9F,9F,C3,7E,00 ' s",
            '520 FORF%=&H2000+ASC("a")*8TO&H2097+ASC("a")*8:READA$:VPOKEF%,VAL("&H"+A$):NEXTF%:F%=USR2(0):F%=USR3(0)',
            "530 DATA C1,C1,C1,C1,C1,C1,C1,C1 ' a",
            "531 DATA C1,C1,C1,C1,C1,C1,C1,C1 ' b",
            "532 DATA C1,C1,C1,C1,C1,C1,C1,C1 ' c",
            "533 DATA C1,C1,C1,C1,C1,C1,C1,C1 ' d",
            "534 DATA D1,D1,41,71,71,41,D1,D1 ' e",
            "535 DATA A1,A1,A1,A1,A1,A1,A1,A1 ' f",
            "536 DATA F1,F1,F1,F1,D1,D1,D1,D1 ' g",
            "537 DATA D1,D1,D1,D1,D1,D1,D1,D1 ' h",
            "540 DATA D1,D1,21,21,21,21,41,41 ' i",
            "541 DATA 31,31,D1,D1,81,81,51,51 ' j",
            "542 DATA F1,F1,F1,21,21,F1,F1,F1 ' k",
            "543 DATA F1,F1,F1,F1,F1,F1,F1,F1 ' l",
            "544 DATA F1,41,41,F1,F1,41,41,F1 ' m",
            "545 DATA D1,D1,81,81,81,81,D1,D1 ' n",
            "546 DATA 21,21,21,21,21,21,21,21 ' o",
            "547 DATA 21,21,21,21,21,21,21,21 ' p",
            "548 DATA 21,21,21,21,21,21,21,21 ' q",
            "550 DATA 21,21,21,21,21,21,21,21 ' r",
            "551 DATA 90,90,90,90,90,90,90,00 ' s",
            "552 DATA 91,91,91,91,91,91,91,91 ' t",
            ]
        ripped_lines = aux_rip_tiles(source_lines,False,False,8,8,rip_flag,False)
        
        self.assertEqual(ripped_lines, \
            [
            '$22,$63,$F7,$B7,$FF,$7E,$3C,$3C',
            '$38,$6C,$FF,$3F,$0F,$3F,$FC,$38',
            '$3C,$3C,$7E,$FF,$ED,$EF,$C6,$44',
            '$1C,$36,$FF,$FC,$F0,$FC,$3F,$1C',
            '$3C,$7E,$FF,$FF,$FF,$FF,$7E,$3C',
            '$08,$10,$54,$FE,$FE,$FE,$FE,$7C',
            '$06,$08,$10,$10,$38,$7C,$7C,$38',
            '$00,$28,$FE,$AA,$FE,$54,$38,$10',
            '$08,$1C,$2A,$7F,$77,$3E,$36,$63',
            '$FF,$D5,$AB,$D5,$AB,$D5,$AB,$FF',
            '$00,$00,$00,$18,$18,$00,$00,$00',
            '$00,$00,$18,$24,$24,$18,$00,$00',
            '$00,$18,$3C,$66,$66,$3C,$18,$00',
            '$3C,$7E,$E7,$C3,$C3,$E7,$7E,$3C',
            '$00,$01,$06,$1D,$2A,$2A,$2A,$1F',
            '$1B,$8F,$65,$11,$C9,$A9,$B1,$F3',
            '$4C,$F7,$F0,$18,$07,$02,$3E,$FE',
            '$7F,$9F,$31,$41,$81,$81,$F9,$FD',
            '$7E,$C3,$9F,$9F,$9F,$C3,$7E,$00',
            '$C1,$C1,$C1,$C1,$C1,$C1,$C1,$C1',
            '$C1,$C1,$C1,$C1,$C1,$C1,$C1,$C1',
            '$C1,$C1,$C1,$C1,$C1,$C1,$C1,$C1',
            '$C1,$C1,$C1,$C1,$C1,$C1,$C1,$C1',
            '$D1,$D1,$41,$71,$71,$41,$D1,$D1',
            '$A1,$A1,$A1,$A1,$A1,$A1,$A1,$A1',
            '$F1,$F1,$F1,$F1,$D1,$D1,$D1,$D1',
            '$D1,$D1,$D1,$D1,$D1,$D1,$D1,$D1'
            ]
        )

    def test_rip_tiles_cpc_basic_1(self):
        rip_flag = True
        self.maxDiff = None
        source_lines = [ \
            "20 SYMBOL AFTER 32",
            "30 REM Upper case chars",
            "40 SYMBOL 65,126,66,66,126,98,98,98,0",
            "50 SYMBOL 66,126,66,66,126,98,98,126,0",
            "60 SYMBOL 67,126,64,64,96,96,96,126,0",
            "70 SYMBOL 68,254,66,66,98,98,98,254,0",
            "80 SYMBOL 69,126,64,64, 120,96,96,126,0",
            "90 SYMBOL 70,126,64,64,120,96,96,96,0",
            "100 SYMBOL 71,126,64,64,102,98,98,126,0",
            "110 SYMBOL 72,66,66,66,126,98,98,98,0",
            "120 SYMBOL 73,60,16,16,24,24,24,60,0",
            "130 SYMBOL 74,126,8,8,24,24,24,120,0",
            "140 SYMBOL 75,68,68,68, 120,100,100,100,0",
            "150 SYMBOL 76,64,64,64,96,96,96, 126,0",
            "160 SYMBOL 77,126,74,74,98,98,98,98,0",
            "170 SYMBOL 78,98,82,74,102,98,98,98,0",
            "180 SYMBOL 79,126,66,66,98,98,98,126,0",
            "190 SYMBOL 80,126,66,66,126,96,96,96,0",
            "200 SYMBOL 81,126,66,66,98,98,106,126,4",
            "210 SYMBOL 82,126,66,66,126,106,100,98,0",
            "220 SYMBOL 83,126,64,64,126,6,6,126,0",
            "230 SYMBOL 84,126,16,16,24,24,24,24,0",
            "240 SYMBOL 85,66,66,66,98,98,98,126,0",
            "250 SYMBOL 86,66,66,66,66,66,36,24,0",
            "260 SYMBOL 87,66,66,66,98,106,106,126,0",
            "270 SYMBOL 88,102,102,36,24,36,102,102,0",
            "280 SYMBOL 89,66,66,126,16,24,24,24,0",
            "290 SYMBOL 90,126,4,8,16,32,64,126,0",
            "295 REM Lower case chars",
            "300 SYMBOL 97,0,0,126,6,126,70,126,0",
            "310 SYMBOL 98,96,96,96,126,98,98,126,0",
            "320 SYMBOL 99,0,0,126,96,96,96,126,0",
            "330 SYMBOL 100,6,6,6,126,70,70,126,0",
            "340 SYMBOL 101,0,0,126,98,126,96,126,0",
            "350 SYMBOL 102,60,48,48,120,48,48,48,0",
            "360 SYMBOL 103,0,0,126,70,70,126,6,126",
            "370 SYMBOL 104,96,96,96,126,98,98,98,0",
            "380 SYMBOL 105,24,0,24,24,24,24,24,0",
            "390 SYMBOL 106,6,0,6,6,6,6,6,126",
            "400 SYMBOL 107,96,96,102,108,120,108, 102,0",
            "410 SYMBOL 108,24,24,24,24,24,24,24,0",
            "420 SYMBOL 109,0,0,126,90,90,66,66,0",
            "430 SYMBOL 110,0,0,108,114,98,98,98,0",
            "440 SYMBOL 111,0,0,126,102,102,102,126,0",
            "450 SYMBOL 112,0,0,126,98,98,126,96,96",
            "460 SYMBOL 113,8,0,126,70,70,126,6,6",
            "470 SYMBOL 114,0,0,108,114,96,96,96,0",
            "480 SYMBOL 115,0,0,126,96,126,6,126,0",
            "490 SYMBOL 116,24,62,24,24,24,24,30,0",
            "500 SYMBOL 117,0,0,102,102,102,102,126,0",
            "510 SYMBOL 118,0,0,102,102,102,60,24,0",
            "520 SYMBOL 119,0,0,66,66,90,90,126,0",
            "530 SYMBOL 120,0,0,198,104,16,104,198,0",
            "540 SYMBOL 121,0,0,102,102,102,126,6,126",
            "550 SYMBOL 122,0,0,126,12,24,48,126,0",
            "555 REM Numbers",
            "560 SYMBOL 48,126,102,110,118,102,102,126,0",
            "570 SYMBOL 49,24,56,24,24,24,24,126,0",
            "580 SYMBOL 50,126,2,2,126,96,96,126,0",
            "590 SYMBOL 51,126,2,2,30,6,6,126,0",
            "600 SYMBOL 52,96,96,96,96,104,126,8,8",
            "610 SYMBOL 53,126,64,126,6,6,6,126,0",
            "620 SYMBOL 54,126,64,64,126,98,98,126,0",
            "630 SYMBOL 55,126,2,4,62,16,32,64,0",
            "640 SYMBOL 56,126,66,66,126,66,66,126,0",
            "650 SYMBOL 57,126,66,66,126,6,6,6,0",
            "680 SYMBOL 95,0,255,0,0,0,0,0,0",
            ]
        ripped_lines = aux_rip_tiles(source_lines,False,False,8,8,rip_flag,False)
        
        self.assertEqual(ripped_lines, \
            [
            '126,66,66,126,98,98,98,0',
            '126,66,66,126,98,98,126,0',
            '126,64,64,96,96,96,126,0',
            '254,66,66,98,98,98,254,0',
            '126,64,64,120,96,96,126,0',
            '126,64,64,120,96,96,96,0',
            '126,64,64,102,98,98,126,0',
            '66,66,66,126,98,98,98,0',
            '60,16,16,24,24,24,60,0',
            '126,8,8,24,24,24,120,0',
            '68,68,68,120,100,100,100,0',
            '64,64,64,96,96,96,126,0',
            '126,74,74,98,98,98,98,0',
            '98,82,74,102,98,98,98,0',
            '126,66,66,98,98,98,126,0',
            '126,66,66,126,96,96,96,0',
            '126,66,66,98,98,106,126,4',
            '126,66,66,126,106,100,98,0',
            '126,64,64,126,6,6,126,0',
            '126,16,16,24,24,24,24,0',
            '66,66,66,98,98,98,126,0',
            '66,66,66,66,66,36,24,0',
            '66,66,66,98,106,106,126,0',
            '102,102,36,24,36,102,102,0',
            '66,66,126,16,24,24,24,0',
            '126,4,8,16,32,64,126,0',
            '0,0,126,6,126,70,126,0'
            ]
        )


    def test_rip_tiles_atari_basic_1(self):
        rip_flag = True
        self.maxDiff = None
        source_lines = [ \
            '0A=PEEK(106)-4:POK.106,A:GR.17:U=256*A:F.I=28TO432:POK.U+I,PEEK(57344+I):N.I:F.I=64TO79:REA.J:POK.U+I,J:N.I:H=400',
            '1DIMN(255):POK.756,A:M=47:F.I=0TO4:REA.L:POK.708+I,L:N.I:B=PEEK(88)+PEEK(89)*256:O=B+59:Z=125:D=B+460:T=B+210:E=10',
            '2P=T:L=26:?#6;CHR$(Z);"USE IJKL SPC":K=1:Q=.9:G=B:U=3:J=0:F.I=0TO127:N(I)=0:N(I+128)=0:POK.O+1+INT(RND(0)*H),M:N.I',
            '3N(0)=1:N(5)=20:N(13)=-20:N(1)=-1:W=132:POS.6,1:?#6;" MINES+ ":F.I=0TO19:POK.O-I,73:POK.D+I,73:N.I:D.0,60,126,126',
            '4R=J*(J>R)+R*(J<=R):POS.1,0:?#6;9-F;" ";CHR$(136);U;"  ";J;"  hi ";R:IFPEEK(P)=W THENJ=J+9:F=F-(F>0):SO.0,Z,E,E',
            '5SE.4,0,0:Y=0:IFQ<RND(0)THENPOK.P-4+2*INT(RND(1)*5)+40*INT(RND(1)*3)-40,M:IFJ>=K*500 THENU=U+(U<9):K=K+1:F=0:L=74',
            '6POK.P,200:S=PEEK(764):SO.0,0,0,0:IFS<>9THENPOK.764,9:IFN(S)THENPOK.P,M:P=P+N(S):J=J+1:Y=1:Q=Q-.0005:SO.0,99,E,E',
            '7POK.G,W:G=B:POK.729,0:IFY*(INT(J/4)=J/4)THENG=O+1+INT(RND(1)*H):POK.G,68:D.126,126,60,0,0,170,85,170,85,170,85',
            '8U=(P>O)*(P<D)*U:IFS=33ANDF<9THENPOK.P-1,0:POK.P+1,0:POK.P-20,0:POK.P+20,0:F=F+1:SE.4,33,0:G.3:D.0,14,120,30,30,0',
            '9POK.711,L:ON((PEEK(P)<>M)*(U>0))GOTO4:SO.0,99,8,9:POK.P,138:U=U-1:F=0:ON(U>0)GOTO4:SO.0,0,0,0:F.I=0TO2600:N.I:G.2',
            ]
        ripped_lines = aux_rip_tiles(source_lines,False,False,8,8,rip_flag,False)
        
        self.assertEqual(ripped_lines, \
            [
            '0,60,126,126,126,126,60,0', 
            '0,170,85,170,85,170,85,0'
            ]
        )


    def test_rip_tiles_bbc_basic_1(self):
        rip_flag = True
        self.maxDiff = None
        source_lines = [ \
            '10 REM Walking alien',
            '20 MODE 6',
            '30 VDU 23,240,153,189,219,126,36,60,36,36',
            '40 PRINT TAB(0,10);CHR$(240)',
            '50 FOR I%=1 TO 19',
            '60   PRINT TAB(I%-1,10);" "',
            '70  PRINT TAB(I%,10);CHR$(240)',
            '80  FOR J%=1 TO 500: NEXT J%',
            '90 NEXT I%',
            '100 END',
            ]
        ripped_lines = aux_rip_tiles(source_lines,False,False,8,8,rip_flag,False)
        
        self.assertEqual(ripped_lines, \
            [
            '153,189,219,126,36,60,36,36'
            ]
        )


    def test_rip_tiles_sinclair_basic_1(self):
        rip_flag = True
        self.maxDiff = None
        source_lines = [ \
            '5 POKE USR "a"+0,BIN 0000000',
            '10 POKE USR "a"+1,BIN 0001000',
            '20 POKE USR "a"+2,BIN 0011100',
            '30 POKE USR "a"+3,BIN 0111110',
            '40 POKE USR "a"+4,BIN 1111111',
            '50 POKE USR "a"+5,BIN 0111110',
            '60 POKE USR "a"+6,BIN 0011100',
            '70 POKE USR "a"+7,BIN 0001000',
            ]
        ripped_lines = aux_rip_tiles(source_lines,False,False,8,8,rip_flag,False)
        
        self.assertEqual(ripped_lines, \
            [
            '0,8,28,62,127,62,28,8'
            ]
        )

    # compute tile
    def test_compute_tile_1(self):
        source_lines = [ \
        '...##...\n', 
        '..#..#..\n', 
        '...##...\n', 
        '.##..##.\n', 
        '#.####.#\n', 
        '# ####.#\n', 
        '..#..#..\n', 
        '..#..#..\n'
        ]
        tile,xsize,ysize = compute_tile(source_lines)
        self.assertEqual(tile, \
            '24,36,24,102,189,189,36,36'
        )
        self.assertEqual(xsize,8)
        self.assertEqual(ysize,8)


    # compute split tile
    def test_compute_split_tile_1(self):
        self.maxDiff = None

        source_lines = [ \
        '...######.......\n', 
        '.###....###.....\n', 
        '##.....#..##....\n', 
        '#..#####...#....\n', 
        '#...#..#...#....\n', 
        '#...#####..#....\n', 
        '##..#.....##....\n', 
        '.###....###.....\n', 
        '...######.......\n', 
        '................\n', 
        '................\n', 
        '................'
        ]
    
        split_tiles = compute_split_tiles(source_lines,False)
        self.assertEqual(split_tiles, \
        [
            [
            '...#####',
            '.###....',
            '##.....#',
            '#..#####',
            '#...#..#',
            '#...####',
            '##..#...',
            '.###....',
            '...#####',
            '........',
            '........',
            '........'
            ],
            [
            '#.......',
            '###.....',
            '..##....',
            '...#....',
            '...#....',
            '#..#....',
            '..##....',
            '###.....',
            '#.......',
            '........',
            '........',
            '........'
            ]
        ]      
        )


    def test_compute_split_tile_2(self):
        self.maxDiff = None

        source_lines = [ \
        '...######.......\n', 
        '.###....###.....\n', 
        '.#..#.....#.....\n', 
        '##..##....##....\n', 
        '#...######.#....\n', 
        '#...#..##..#....\n', 
        '#..##..#...#....\n', 
        '#.######...#....\n', 
        '##....##..##....\n', 
        '.#.....#..#.....\n', 
        '.###....###.....\n', 
        '...######.......\n', 
        '................\n', 
        '................\n', 
        '................\n', 
        '................'
        ]
        split_tiles = compute_split_tiles(source_lines,False)
        self.assertEqual(split_tiles, \
        [
            [
            '...#####',
            '.###....',
            '.#..#...',
            '##..##..',
            '#...####',
            '#...#..#',
            '#..##..#',
            '#.######',
            '##....##',
            '.#.....#',
            '.###....',
            '...#####',
            '........',
            '........',
            '........',
            '........'
            ],
            [
            '#.......',
            '###.....',
            '..#.....',
            '..##....',
            '##.#....',
            '#..#....',
            '...#....',
            '...#....',
            '..##....',
            '..#.....',
            '###.....',
            '#.......',
            '........',
            '........',
            '........',
            '........'
            ]
        ]
        )


    # trim_newline_from_shape
    def test_trim_newline_from_shape_1(self):
        source_lines = ['#.####.#\n', '.######.\n', '#..##..#\n', '##.##.##\n', '.######.\n', '..####..\n', '.######.\n', '##.##.##\n']
        
        trimmed_lines,xsize,ysize = trim_newline_from_shape(source_lines)

        self.assertEqual(trimmed_lines, ['#.####.#', '.######.', '#..##..#', '##.##.##', '.######.', '..####..', '.######.', '##.##.##'])
        self.assertEqual(xsize,8)
        self.assertEqual(ysize,8)


    def test_trim_newline_from_shape_2(self):
        source_lines = ['\n', '#.####.#\n', '.######.\n', '#..##..#\n', '##.##.##\n', '.######.\n', '..####..\n', '.######.\n', '##.##.##\n', '\n', '#############']
        
        trimmed_lines,xsize,ysize = trim_newline_from_shape(source_lines)

        self.assertEqual(trimmed_lines, ['#.####.#', '.######.', '#..##..#', '##.##.##', '.######.', '..####..', '.######.', '##.##.##','########'])
        self.assertEqual(xsize,8)
        self.assertEqual(ysize,9)






if __name__ == '__main__':
    global_vars.verbose = 0
    global_vars.test = 1
    unittest.main()
