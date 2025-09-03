import unittest
import sys



sys.path.append("modules")

from params import *



class testParams(unittest.TestCase):

    def test_handle_two_letter_params_1(self):
        params = ("./xl", "tu")
        result = handle_two_letter_params(params)
        
        self.assertEqual(result,['', 'test', 'unit_tests'])

    def test_handle_two_letter_params_2(self):
        params = ("./xl", "hu")
        result = handle_two_letter_params(params)
        
        self.assertEqual(result,['', 'help', 'unit_tests'])

    def test_get_size_params_1(self):
        params = ("size", "helloworld", "25", "32")
        result = get_size_params(params)
        
        self.assertEqual(result,("ncurses_sized", "25", "32"))

    def test_get_size_params_2(self):
        params = ['size', 'helloworld', 'terminal', '20', '40']
        result = get_size_params(params)
        
        self.assertEqual(result,('terminal8x8_sized', '20', '40') )




if __name__ == '__main__':
    
    unittest.main()
