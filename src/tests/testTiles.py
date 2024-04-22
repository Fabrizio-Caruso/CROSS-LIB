import unittest
import sys



sys.path.append("assets")

# from generate_assets import *

from tiles import *



class testTiles(unittest.TestCase):


    # detect_xsize
    def test_detect_xsize_1(self):
        res = detect_xsize("vic20")
        self.assertEqual(res,8)

    def test_detect_xsize_2(self):
        res = detect_xsize("oric")
        self.assertEqual(res,6)

    def test_detect_xsize_3(self):
        res = detect_xsize("pecom")
        self.assertEqual(res,6)

    def test_detect_xsize_4(self):
        res = detect_xsize("comx")
        self.assertEqual(res,6)

    def test_detect_xsize_5(self):
        res = detect_xsize("micro")
        self.assertEqual(res,6)

    def test_detect_xsize_6(self):
        res = detect_xsize("atari_lynx")
        self.assertEqual(res,8)

    def test_detect_xsize_7(self):
        res = detect_xsize("apple2")
        self.assertEqual(res,7)

    def test_detect_xsize_8(self):
        res = detect_xsize("apple2enh")
        self.assertEqual(res,7)
        
    def test_detect_xsize_9(self):
        res = detect_xsize("comx_ntsc")
        self.assertEqual(res,6)

    def test_detect_xsize_10(self):
        res = detect_xsize("micro_ntsc")
        self.assertEqual(res,6)

    def test_detect_xsize_11(self):
        res = detect_xsize("atmos")
        self.assertEqual(res,6)


    # detect_ysize
    def test_detect_ysize_1(self):
        res = detect_ysize("vic20")
        self.assertEqual(res,8)

    def test_detect_ysize_2(self):
        res = detect_ysize("oric")
        self.assertEqual(res,8)

    def test_detect_ysize_3(self):
        res = detect_ysize("pecom")
        self.assertEqual(res,9)

    def test_detect_ysize_4(self):
        res = detect_ysize("comx")
        self.assertEqual(res,9)

    def test_detect_ysize_5(self):
        res = detect_ysize("micro")
        self.assertEqual(res,9)

    def test_detect_ysize_6(self):
        res = detect_ysize("atari_lynx")
        self.assertEqual(res,6)

    def test_detect_ysize_7(self):
        res = detect_ysize("apple2")
        self.assertEqual(res,8)

    def test_detect_ysize_8(self):
        res = detect_ysize("apple2enh")
        self.assertEqual(res,8)
        
    def test_detect_ysize_9(self):
        res = detect_ysize("comx_ntsc")
        self.assertEqual(res,8)

    def test_detect_ysize_10(self):
        res = detect_ysize("micro_ntsc")
        self.assertEqual(res,8)

    def test_detect_ysize_11(self):
        res = detect_ysize("atmos")
        self.assertEqual(res,8)


    # convert
    def test_convert_1(self):
    
        tile_shape = \
        [
        "...##...",
        "..#..#..", 
        "...##...", 
        ".##..##.", 
        "#..##..#",
        "...##...",
        "..#..#..",
        ".##..##."
        ]

        res = convert(tile_shape,8,6,"zero")
        
        expected = \
        [
        '..#..#..', 
        '...##...', 
        '.##..##.', 
        '#..##..#', 
        '...##...', 
        '..#..#..'
        ]
        self.assertEqual(res,expected)

    def test_convert_2(self):
    
        tile_shape = \
        [
        "...##...",
        "..#..#..", 
        "...##...", 
        ".##..##.", 
        "#..##..#",
        "...##...",
        "..#..#..",
        ".##..##."
        ]

        res = convert(tile_shape,8,6,"duplicate")
        
        expected = \
        [
        '..#..#..', 
        '...##...', 
        '.##..##.', 
        '#..##..#', 
        '...##...', 
        '..#..#..'
        ]
        self.assertEqual(res,expected)

    def test_convert_3(self):
    
        tile_shape = \
        [
        "...##...",
        "..#..#..", 
        "...##...", 
        ".##..##.", 
        "#..##..#",
        "...##...",
        "..#..#..",
        ".##..##."
        ]

        res = convert(tile_shape,6,9,"zero")
        
        expected = \
        [
        '..##..',
        '.#..#.',
        '..##..',
        '##..##',
        '..##..',
        '..##..',
        '.#..#.',
        '##..##',
        '......'
        ]

        self.assertEqual(res,expected)

    def test_convert_4(self):
    
        tile_shape = \
        [
        "...##...",
        "..#..#..", 
        "...##...", 
        ".##..##.", 
        "#..##..#",
        "...##...",
        "..#..#..",
        ".##..##."
        ]

        res = convert(tile_shape,6,9,"duplicate")
        
        expected = \
        [
        '..##..',
        '.#..#.',
        '..##..',
        '##..##',
        '..##..',
        '..##..',
        '.#..#.',
        '##..##',
        '##..##'
        ]
        self.assertEqual(res,expected)

    def test_convert_5(self):
    
        tile_shape = \
        [
        "...##...",
        "..#..#..", 
        "...##...", 
        ".##..##.", 
        "#..##..#",
        "...##...",
        "..#..#..",
        ".##..##."
        ]

        res = convert(tile_shape,6,8,"duplicate")
        
        expected = \
        [
        '..##..',
        '.#..#.',
        '..##..',
        '##..##',
        '..##..',
        '..##..',
        '.#..#.',
        '##..##',
        ]
        self.assertEqual(res,expected)

    def test_convert_6(self):
    
        tile_shape = \
        [
        "...##...",
        "..#..#..", 
        "...##...", 
        ".##..##.", 
        "#..##..#",
        "...##...",
        "..#..#..",
        ".##..##."
        ]

        res = convert(tile_shape,7,8,"duplicate")
        
        expected = \
        [
        '...##..',
        '..#..#.',
        '...##..',
        '.##..##',
        '#..##..',
        '...##..',
        '..#..#.',
        '.##..##'
        ]

        self.assertEqual(res,expected)
        

    def test_convert_7(self):
    
        tile_shape = \
        [
        "...##...",
        "..#..#..", 
        "...##...", 
        ".##..##.", 
        "#..##..#",
        "...##...",
        "..#..#..",
        ".##..##."
        ]

        res = convert(tile_shape,6,10,"duplicate")
        
        expected = \
        [
        '..##..',
        '..##..',
        '.#..#.',
        '..##..',
        '##..##',
        '..##..',
        '..##..',
        '.#..#.',
        '##..##',
        '##..##'
        ]
        self.assertEqual(res,expected)

    def test_convert_8(self):
    
        tile_shape = \
        [
        "...##...",
        "..#..#..", 
        "...##...", 
        ".##..##.", 
        "#..##..#",
        "...##...",
        "..#..#..",
        ".##..##."
        ]

        res = convert(tile_shape,6,10,"zero")
        
        expected = \
        [
        '......',
        '..##..',
        '.#..#.',
        '..##..',
        '##..##',
        '..##..',
        '..##..',
        '.#..#.',
        '##..##',
        '......'
        ]
        self.assertEqual(res,expected)

    def test_convert_9(self):
    
        tile_shape = \
        [
        "...##...",
        "..#..#..", 
        "...##...", 
        ".##..##.", 
        "#..##..#",
        "...##...",
        "..#..#..",
        ".##..##."
        ]

        res = convert(tile_shape,8,10,"duplicate")
        
        expected = \
        [
        '...##...',
        "...##...",
        "..#..#..", 
        "...##...", 
        ".##..##.", 
        "#..##..#",
        "...##...",
        "..#..#..",
        ".##..##.",
        '.##..##.'
        ]
        self.assertEqual(res,expected)

    def test_convert_10(self):
    
        tile_shape = \
        [
        "...##...",
        "..#..#..", 
        "...##...", 
        ".##..##.", 
        "#..##..#",
        "...##...",
        "..#..#..",
        ".##..##."
        ]

        res = convert(tile_shape,8,10,"zero")
        
        expected = \
        [
        '........',
        "...##...",
        "..#..#..", 
        "...##...", 
        ".##..##.", 
        "#..##..#",
        "...##...",
        "..#..#..",
        ".##..##.",
        '........'
        ]
        self.assertEqual(res,expected)



if __name__ == '__main__':
    # global_vars.verbose = 0
    # global_vars.test = 1
    
    unittest.main()
