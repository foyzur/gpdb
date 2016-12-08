import os
import inspect

import tinctest
from tinctest import TINCTestCase

class verify(TINCTestCase):

    def test_max_changepriority(self):
        out_file = self._get_absolute_filename('./execorder.out')
        try:
            with open(out_file, 'r') as f:
                first_line = f.readline()
                self.failUnless(first_line.startswith('change_priority.sql'), 'Higher priority query did not complete first')
        except IOError:
            print 'Output file does not exist'
            raise

