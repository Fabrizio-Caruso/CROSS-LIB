import unittest
import sys

# sys.path.append("../assets/")
# from import_from_source import *
sys.path.append("assets")

# from import_from_source import *

from import_from_source import *

class testStrings(unittest.TestCase):

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


    def intListToString(intList):
        res = ""
        for value in intList:
            res+=str(value)+","
        res=res[:-1]
        return res  

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
        
        initial_list_string = testStrings.intListToString(initial_list)
        
        shape0 = compute_shape(initial_list_string,8)
        expected_result = [0, 0, 0, 0, 128, 120, 102, 85]   
        self.assertEqual(compute_rotated_shape(shape0),expected_result)
        expected_result_string = testStrings.intListToString(expected_result)

        shape1 = compute_shape(expected_result_string,8)
        expected_result = [16, 224, 96, 160, 32, 192, 64, 128]
        self.assertEqual(compute_rotated_shape(shape1), expected_result)
        expected_result_string = testStrings.intListToString(expected_result)       
        
        shape2 = compute_shape(expected_result_string,8)
        expected_result = [170, 102, 30, 1, 0, 0, 0, 0]
        self.assertEqual(compute_rotated_shape(shape2), expected_result)
        expected_result_string = testStrings.intListToString(expected_result)
        
        shape3 = compute_shape(expected_result_string,8)
        expected_result = initial_list 
        self.assertEqual(compute_rotated_shape(shape3), expected_result)  
        expected_result_string = testStrings.intListToString(expected_result)

    def test_compute_rotated_shape_2(self):
       
        initial_list = [255,0,255,0,255,0,255,0]
        
        initial_list_string = testStrings.intListToString(initial_list)
        
        shape0 = compute_shape(initial_list_string,8)
        expected_result = [85, 85, 85, 85, 85, 85, 85, 85]   
        self.assertEqual(compute_rotated_shape(shape0),expected_result)
        expected_result_string = testStrings.intListToString(expected_result)

        shape1 = compute_shape(expected_result_string,8)
        expected_result = [0,255,0,255,0,255,0,255]
        self.assertEqual(compute_rotated_shape(shape1), expected_result)
        expected_result_string = testStrings.intListToString(expected_result)       
        
        shape2 = compute_shape(expected_result_string,8)
        expected_result = [170, 170, 170, 170, 170, 170, 170, 170]
        self.assertEqual(compute_rotated_shape(shape2), expected_result)
        expected_result_string = testStrings.intListToString(expected_result)
        
        shape3 = compute_shape(expected_result_string,8)
        expected_result = initial_list 
        self.assertEqual(compute_rotated_shape(shape3), expected_result)  
        expected_result_string = testStrings.intListToString(expected_result)


    def test_rip_tiles_asm_1(self):
        source_lines = ['!byte 0,0,60,98,126,98,98,98\n']
        ripped_lines = aux_rip_tiles(source_lines,False,False,8,8,False,False)
        
        self.assertEqual(ripped_lines,['0,0,60,98,126,98,98,98'])

    def test_rip_tiles_asm_2(self):
        source_lines = ['!byte 0x00, 0x1F, 0x28, 0x28, 0x28, 0x3F, 0x1F, 0x00\n']
        ripped_lines = aux_rip_tiles(source_lines,False,False,8,8,False,False)
        
        self.assertEqual(ripped_lines,['$00,$1F,$28,$28,$28,$3F,$1F,$00'])
        
    def test_rip_tiles_cbm_basic_1(self):
        rip_flag = False
        source_lines = [ \
            '000 REM ** USER DEFINED GRAPHICS **',
            '001 REM ** BY SHANE JOLLY **',
            '100 PRINT "♥": PRINT TAB(12)"PLEASE WAIT..."',
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

if __name__ == '__main__':
    unittest.main()
