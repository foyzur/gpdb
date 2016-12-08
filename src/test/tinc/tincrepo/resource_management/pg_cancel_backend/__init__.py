import os
import sys
import datetime
import math
import tinctest
import re
from time import gmtime, strftime, sleep

import unittest2 as unittest
from fnmatch import fnmatch

from tinctest.runner import TINCTextTestResult
from tinctest.lib import run_shell_command, Gpdiff

from mpp.lib.PSQL import PSQL
from mpp.models import SQLTestCase

class PgCancelBackendTestCaseSQLTestCase(SQLTestCase):

    def _infer_metadata(self):
        super(PgCancelBackendTestCaseSQLTestCase, self)._infer_metadata()
        self.querylookup = self._metadata.get('querylookup', 'select')

        
    def run_test(self):
        sql_file = self.sql_file
        ans_file = self.ans_file
        
        source_file = sys.modules[self.__class__.__module__].__file__
        source_dir = os.path.dirname(source_file)
        out_dir = self.get_out_dir()

        cancel_out = os.path.join(out_dir, os.path.basename(sql_file).replace('.sql', '_cancel.out'))
        verification_file = os.path.join(out_dir, os.path.basename(sql_file).replace('.sql', '.out'))

        cmd = "psql " +self.db_name+ " -f " +sql_file+ " &> " +cancel_out+ "&"
        run_shell_command(cmd, "run file until it's canceled")

        sql = "select procpid from pg_stat_activity where current_query not like '%current_query%' AND current_query like '%" +self.querylookup+ "%'"
        out = PSQL.run_sql_command(sql, dbname = self.db_name)
        out = out.split('\n')
        pid = []
        for line in out:
            line = line.strip()
            if re.match('^[0-9]+',line):
                pid.append(line.strip())

        if len(pid) != 1:
            #at this point there're multiple query that fits the description or no queries at all, we wouldn't know what to kill, thus the test fail
            tinctest.logger.error("multiple or no query returned, failing the test as not sure what to do now")
            return False

        sleep(10)

        cancelquery = "select pg_cancel_backend(" +pid[0]+ ")"
        PSQL.run_sql_command(cancelquery, dbname = self.db_name)

        #ping output file every second until statement is canceled, 100 retries
        ready = False
        timeout = 0
        while ready == False:
            syscmd = "grep \"canceling statement\" " +cancel_out+ "|wc -l"
            p = os.popen(syscmd, 'r')
            out = p.readline()
            if int(out.strip()) > 0:
                ready = True
            else:
                if timeout == 100:
                    tinctest.logger.error("waited too long for the query to cancel, failing the test")
                    return False
                tinctest.logger.info("waiting for query to disappear from pg_stat_activity")
                timeout += 1
                sleep(1)

        #now verify process no longer exist
        cmd = "psql " +self.db_name+ " -c \"select * from pg_stat_activity where procpid = " +pid[0]+ "\" > " +verification_file
        run_shell_command(cmd, "check to see if process still exist")

        return Gpdiff.are_files_equal(verification_file, ans_file )
           

