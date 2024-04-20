import unittest
import sys



sys.path.append("assets")

# from generate_assets import *

from generate_assets import *



class testAssets(unittest.TestCase):

    COLOR1 = 0x55
    COLOR2 = 0xAA
    COLOR3 = 0xFF

    def test_four_colored_1(self):
        res = four_colored(0x1,testAssets.COLOR3)
        self.assertEqual(res,0x03)

    def test_four_colored_2(self):
        res = four_colored(0x3,testAssets.COLOR3)
        self.assertEqual(res,0x0F)

    def test_four_colored_3(self):
        res = four_colored(0x7,testAssets.COLOR3)
        self.assertEqual(res,0x3F)

    def test_four_colored_4(self):
        res = four_colored(0xF,testAssets.COLOR3)
        self.assertEqual(res,0xFF)

    def test_four_colored_5(self):
        res = four_colored(0x1,testAssets.COLOR2)
        self.assertEqual(res,0x02)

    def test_four_colored_6(self):
        res = four_colored(0x3,testAssets.COLOR2)
        self.assertEqual(res,0x0A)

    def test_four_colored_7(self):
        res = four_colored(0x7,testAssets.COLOR2)
        self.assertEqual(res,0x2A)

    def test_four_colored_8(self):
        res = four_colored(0xF,testAssets.COLOR2)
        self.assertEqual(res,0xAA)

    def test_four_colored_9(self):
        res = four_colored(0x1,testAssets.COLOR1)
        self.assertEqual(res,0x01)

    def test_four_colored_10(self):
        res = four_colored(0x3,testAssets.COLOR1)
        self.assertEqual(res,0x05)

    def test_four_colored_11(self):
        res = four_colored(0x7,testAssets.COLOR1)
        self.assertEqual(res,0x15)

    def test_four_colored_12(self):
        res = four_colored(0xF,testAssets.COLOR1)
        self.assertEqual(res,0x55)

    def test_four_colored_13(self):
        res = four_colored(0x2,testAssets.COLOR1)
        self.assertEqual(res,0x04)

    def test_four_colored_14(self):
        res = four_colored(0x2,testAssets.COLOR2)
        self.assertEqual(res,0x08)

    def test_four_colored_15(self):
        res = four_colored(0x2,testAssets.COLOR3)
        self.assertEqual(res,0x0C)




if __name__ == '__main__':
    # global_vars.verbose = 0
    # global_vars.test = 1
    unittest.main()
