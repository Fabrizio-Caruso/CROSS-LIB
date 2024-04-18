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
        shape = [
            ".......#",
            "......#.",
            "......##",
            ".....#..",
            ".....#.#",
            ".....##.",
            ".....###",
            "....#...",
            ]
        # print_shape(shape)
        # print(compute_rotated_shape(shape))
        
        expected_result = [0, 0, 0, 0, 128, 120, 102, 85]
        
        self.assertEqual(compute_rotated_shape(shape),expected_result)
        
        
        # print("expected: " + str(expected_result))
        
        rotated_shape = compute_shape("0, 0, 0, 0, 128, 120, 102, 85",8)
        # print_shape(rotated_shape)
        
        expected_result = [16, 224, 96, 160, 32, 192, 64, 128]
        
        self.assertEqual(compute_rotated_shape(rotated_shape), expected_result)
        
        rerotated_shape = compute_shape("16, 224, 96, 160, 32, 192, 64, 128",8)
        # print_shape(rerotated_shape)

if __name__ == '__main__':
    unittest.main()
