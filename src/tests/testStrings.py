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


    def test_rip_tiles_1(self):
        source_lines = ['!byte 0,0,60,98,126,98,98,98\n']
        ripped_lines = aux_rip_tiles(source_lines,False,False,8,8,False,False)
        
        self.assertEqual(ripped_lines,['0,0,60,98,126,98,98,98'])

    def test_rip_tiles_2(self):
        source_lines = ['!byte 0x00, 0x1F, 0x28, 0x28, 0x28, 0x3F, 0x1F, 0x00\n']
        ripped_lines = aux_rip_tiles(source_lines,False,False,8,8,False,False)
        
        self.assertEqual(ripped_lines,['$00,$1F,$28,$28,$28,$3F,$1F,$00'])

if __name__ == '__main__':
    unittest.main()
