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


    def test_compute_rotated_shape_1(self):
    
        def intListToString(intList):
            res = ""
            for value in intList:
                res+=str(value)+","
            res=res[:-1]
            return res
       
        initial_list = [1,2,3,4,5,6,7,8]
            # .......#
            # ......#.
            # ......##
            # .....#..
            # .....#.#
            # .....##.
            # .....###
            #....#...
        
        initial_list_string = intListToString(initial_list)
        
        shape0 = compute_shape(initial_list_string,8)
        expected_result = [0, 0, 0, 0, 128, 120, 102, 85]   
        self.assertEqual(compute_rotated_shape(shape0),expected_result)
        expected_result_string = intListToString(expected_result)

        shape1 = compute_shape(expected_result_string,8)
        expected_result = [16, 224, 96, 160, 32, 192, 64, 128]
        self.assertEqual(compute_rotated_shape(shape1), expected_result)
        expected_result_string = intListToString(expected_result)       
        
        shape2 = compute_shape(expected_result_string,8)
        expected_result = [170, 102, 30, 1, 0, 0, 0, 0]
        self.assertEqual(compute_rotated_shape(shape2), expected_result)
        expected_result_string = intListToString(expected_result)
        
        shape3 = compute_shape(expected_result_string,8)
        expected_result = initial_list 
        self.assertEqual(compute_rotated_shape(shape3), expected_result)  
        expected_result_string = intListToString(expected_result)


if __name__ == '__main__':
    unittest.main()
