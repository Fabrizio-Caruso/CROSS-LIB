import unittest
import sys


sys.path.append("modules")

from option_functions import OptionConfig
from default_values import *

default_option_config = OptionConfig()

class testDefaults(unittest.TestCase):

    # no_default
    def test_no_defaults_1(self):
        params = ("build", "helloworld")
        result = insert_default_sizes(default_option_config, params)
        
        self.assertEqual(result,params)

    def test_no_defaults_2(self):
        params = ("build", "tiles", "vic20")
        result = insert_default_sizes(default_option_config, params)
        
        self.assertEqual(result,params)

    def test_terminal_1(self):
        params = ("build", "helloworld", "terminal")
        result = insert_default_sizes(default_option_config, params)
        
        self.assertEqual(result,['build', 'helloworld', 'terminal', '20', '17'])

    def test_terminal_2(self):
        params = ("build", "helloworld","terminal", "vic20")
        result = insert_default_sizes(default_option_config, params)
        
        self.assertEqual(result,['build', 'helloworld', 'terminal8x8', '22', '23'])

    def test_terminal_3(self):
        params = ("build", "helloworld","terminal", "atari")
        result = insert_default_sizes(default_option_config, params)
        
        self.assertEqual(result,['build', 'helloworld', 'terminal8x8', '20', '24'])

    def test_terminal_4(self):
        params = ("build", "helloworld","terminal6x8")
        result = insert_default_sizes(default_option_config, params)
        
        self.assertEqual(result,['build', 'helloworld', 'terminal6x8', '40', '24'])

    def test_terminal_5(self):
        params = ("build", "helloworld","terminal8x6")
        result = insert_default_sizes(default_option_config, params)
        
        self.assertEqual(result,['build', 'helloworld', 'terminal8x6', '20', '17'])

    def test_terminal_6(self):
        params = ("build", "helloworld","terminal7x8")
        result = insert_default_sizes(default_option_config, params)
        
        self.assertEqual(result,['build', 'helloworld', 'terminal7x8', '20', '24'])

    def test_terminal_7(self):
        params = ("build", "helloworld","terminal6x9")
        result = insert_default_sizes(default_option_config, params)
        
        self.assertEqual(result,['build', 'helloworld', 'terminal6x9', '40', '24'])

    def test_terminal_8(self):
        params = ("build", "helloworld","terminal8x8")
        result = insert_default_sizes(default_option_config, params)
        
        self.assertEqual(result,['build', 'helloworld', 'terminal8x8', '20', '17'])

    def test_terminal_9(self):
        params = ("build", "helloworld","terminal8x6", "atari_lynx")
        result = insert_default_sizes(default_option_config, params)
        
        self.assertEqual(result,['build', 'helloworld', 'terminal8x6', '20', '17'])

    def test_terminal_10(self):
        params = ("build", "helloworld", "terminal", "c64")
        result = insert_default_sizes(default_option_config, params)
        
        self.assertEqual(result,['build', 'helloworld', 'terminal8x8', '40', '25'])

    def test_terminal_11(self):
        params = ("build", "helloworld","terminal", "msx")
        result = insert_default_sizes(default_option_config, params)
        
        self.assertEqual(result,['build', 'helloworld', 'terminal8x8', '32', '24'])

    def test_terminal_12(self):
        params = ("build", "helloworld","terminal", "cpc")
        result = insert_default_sizes(default_option_config, params)
        
        self.assertEqual(result,['build', 'helloworld', 'terminal8x8', '20', '25'])

    def test_terminal_13(self):
        params = ("build", "helloworld","terminal", "spectrum")
        result = insert_default_sizes(default_option_config, params)
        
        self.assertEqual(result,['build', 'helloworld', 'terminal8x8', '32', '24'])


if __name__ == '__main__':
    
    unittest.main()
