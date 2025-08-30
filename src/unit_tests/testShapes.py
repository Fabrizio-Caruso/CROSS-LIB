import unittest
import sys


sys.path.append("modules")

from shape_functions import *



class testShapes(unittest.TestCase):


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

        res = resize_shape(tile_shape,8,6,"zero")
        
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

        res = resize_shape(tile_shape,8,6,"duplicate")
        
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

        res = resize_shape(tile_shape,6,9,"zero")
        
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

        res = resize_shape(tile_shape,6,9,"duplicate")
        
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

        res = resize_shape(tile_shape,6,8,"duplicate")
        
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

        res = resize_shape(tile_shape,7,8,"duplicate")
        
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

        res = resize_shape(tile_shape,6,10,"duplicate")
        
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

        res = resize_shape(tile_shape,6,10,"zero")
        
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

        res = resize_shape(tile_shape,8,10,"duplicate")
        
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

        res = resize_shape(tile_shape,8,10,"zero")
        
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

    unittest.main()
