import unittest

class MyTestCase:
    def setUp(self):
       #
    def tearDown(self):
       #
    @unittest
    def test_something(self):
        # Test something here
        self.assertEqual(2 + 2, 4)

    @unittest
    def test_another_thing(self):
        # Test another thing here
        self.assertTrue(True)
        
if __name__ == '__main__':
    unittest.main()
