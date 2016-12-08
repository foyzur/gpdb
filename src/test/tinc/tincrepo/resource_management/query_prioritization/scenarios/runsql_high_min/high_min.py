import datetime

import tinctest
import unittest2 as unittest

from mpp.lib.PSQL import PSQL
from tinctest.lib import Gpdiff
from mpp.models import SQLConcurrencyTestCase

class high_min(SQLConcurrencyTestCase):
    """
    @db_name memory_quota
    """
    def run_test(self):
        sql_file = self.sql_file
        ans_file = self.ans_file
        test_name = self.name.split('.')[1]
        now = datetime.datetime.now()
        timestamp = '%s%s%s%s%s%s%s'%(now.year,now.month,now.day,now.hour,now.minute,now.second,now.microsecond)
        out_file = sql_file.replace('.sql', timestamp + '.out')
        order_out_file = open ( self._get_absolute_filename('./' + 'execorder.out'), 'a' )
        
        PSQL.run_sql_file(sql_file,dbname=self.db_name, out_file = out_file)
        order_out_file.write(list(self.__dict__['sql_file'].split('/')).pop())
        order_out_file.write('\n')
        order_out_file.close()
     
        result = Gpdiff.are_files_equal(out_file, ans_file)
        if result == False:
            self.test_artifacts.append(out_file.replace('.out', '.diff'))

        return result

