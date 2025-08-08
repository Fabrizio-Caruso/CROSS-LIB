import unittest
import sys



sys.path.append("modules")

from default_values import *



class testDefaults(unittest.TestCase):

    # no_default
    def test_no_defaults_1(self):
        params = ("build", "helloworld")
        result = insert_default_sizes(params)
        
        self.assertEqual(result,params)

    def test_no_defaults_2(self):
        params = ("build", "tiles", "vic20")
        result = insert_default_sizes(params)
        
        self.assertEqual(result,params)

    def test_terminal_1(self):
        params = ("build", "helloworld", "terminal")
        result = insert_default_sizes(params)
        
        self.assertEqual(result,['build', 'helloworld', 'terminal', '20', '17'])

    def test_terminal_2(self):
        params = ("build", "helloworld","terminal", "vic20")
        result = insert_default_sizes(params)
        
        self.assertEqual(result,['build', 'helloworld', 'terminal8x8', '22', '23'])

    def test_terminal_3(self):
        params = ("build", "helloworld","terminal", "atari")
        result = insert_default_sizes(params)
        
        self.assertEqual(result,['build', 'helloworld', 'terminal8x8', '20', '24'])

if __name__ == '__main__':
    # global_vars.verbose = 0
    # global_vars.test = 1
    
    unittest.main()
