import unittest
import sys



sys.path.append("assets")

from strings import *



class testStrings(unittest.TestCase):

    # no_space
    def test_no_space_1(self):
        res = no_space(" ABCDEF GH IJ K L MNOP Q R STUVWXYZ  ")
        expected = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        self.assertEqual(res,expected)

    # only_upper_digits_and_space
    def test_only_upper_digits_and_space_1(self):
        res = only_upper_digits_and_space(" ABCDEF GH IJ K L MNOP Q R STUVWXYZ  ")
        expected = " ABCDEF GH IJ K L MNOP Q R STUVWXYZ  "
        self.assertEqual(res,expected)

    def test_only_upper_digits_and_space_2(self):
        res = only_upper_digits_and_space(" AB45678CDEF GH I0123J K L MNOP Q R STUVWXYZ 9 ")
        expected = " AB45678CDEF GH I0123J K L MNOP Q R STUVWXYZ 9 "
        self.assertEqual(res,expected)

    def test_only_upper_digits_and_space_3(self):
        res = only_upper_digits_and_space("123")
        expected = "123"
        self.assertEqual(res,expected)

    def test_only_upper_digits_and_space_4(self):
        res = only_upper_digits_and_space("ABC")
        expected = "ABC"
        self.assertEqual(res,expected)

    def test_only_upper_digits_and_space_5(self):
        res = only_upper_digits_and_space("1_23")
        expected = "123"
        self.assertEqual(res,expected)

    def test_only_upper_digits_and_space_6(self):
        res = only_upper_digits_and_space("ABabcC")
        expected = "ABC"
        self.assertEqual(res,expected)

    # compute_string
    def test_compute_string_1(self):
        res = compute_string("ABC")
        expected = "_XL_A _XL_B _XL_C"
        self.assertEqual(res,expected)
        
    def test_compute_string_2(self):
        res = compute_string("abc")
        expected = "_XL_a _XL_b _XL_c"
        self.assertEqual(res,expected)

    def test_compute_string_3(self):
        res = compute_string("A 1 b")
        expected = '_XL_A _XL_SPACE "1" _XL_SPACE _XL_b'
        self.assertEqual(res,expected)
        
    def test_compute_string_4(self):
        res = compute_string("123")
        expected = '"1" "2" "3"'
        self.assertEqual(res,expected)

    def test_compute_string_5(self):
        res = compute_string(" A 1 b 7")
        expected = '_XL_SPACE _XL_A _XL_SPACE "1" _XL_SPACE _XL_b _XL_SPACE "7"'
        self.assertEqual(res,expected)


if __name__ == '__main__':
    # global_vars.verbose = 0
    # global_vars.test = 1
    
    unittest.main()
