import unittest
import sys
import os



# generate_assets is side-specific (src/assets/ or src_hd/assets/).
# Resolve relative to this file so CWD is irrelevant.
_repo_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
_side_assets = None
for _side in ("src", "src_hd"):
    _candidate = os.path.join(_repo_root, _side, "assets")
    if os.path.exists(os.path.join(_candidate, "generate_assets.py")):
        _side_assets = _candidate
        break
if _side_assets is None:
    raise ImportError("Cannot find generate_assets.py in src/assets or src_hd/assets")
# generate_assets is side-specific (src/assets/ or src_hd/assets/).
# Resolve relative to this file so CWD is irrelevant.
_repo_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
_side_assets = None
for _side in ("src", "src_hd"):
    _candidate = os.path.join(_repo_root, _side, "assets")
    if os.path.exists(os.path.join(_candidate, "generate_assets.py")):
        _side_assets = _candidate
        break
if _side_assets is None:
    raise ImportError("Cannot find generate_assets.py in src/assets or src_hd/assets")
sys.path.insert(0, _side_assets)

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
    unittest.main()
