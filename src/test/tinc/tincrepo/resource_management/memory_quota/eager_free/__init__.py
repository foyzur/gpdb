import datetime
import sys
import os
import re

import unittest2 as unittest
import tinctest
from gppylib.commands.base import Command
from mpp.lib.PSQL import PSQL
from mpp.models import SQLTestCase

class EagerFreeSQLTestCase(SQLTestCase):

    def setUp(self):
        # Create sql and ans files
        self._create_sql_ans_files()

        super(EagerFreeSQLTestCase, self).setUp()

    def _infer_metadata(self):
        super(EagerFreeSQLTestCase, self)._infer_metadata()
        if self._metadata.get('config_params', None) == None:
            self.config_params = list()
        else:
            self.config_params = list(self._metadata['config_params'].split(','))
        self.statement_mem_list = set(self._metadata['statement_mem'].split(','))
        self.sql_file = self._metadata.get('sql_file')

    def _create_sql_ans_files(self):
        """ 
        Generate test sql and ans files 
        """ 
        test_name = self.name.split('.')[0]
        
        # Form SQL file to run using the query file given as input
        Command('chmod sql file',
                'chmod 777 ' + self._get_absolute_filename(test_name + '/' + test_name + '.sql')).run(validateAfter=True) 
        testSQLFile = open(self._get_absolute_filename(test_name + '/' + test_name + '.sql'), 'w')
        
        # Print operator memory limits 
        testSQLFile.write( "set gp_resqueue_print_operator_memory_limits=on;" )
        testSQLFile.write( "\n\n" )

        # Additional confs
        if self.config_params is not None:
            testSQLFile.write( "--setting additional confs\n\n" )
            for conf in self.config_params:
                testSQLFile.write( "set " + conf + ";" )
                testSQLFile.write( "\n\n" )
  
        # Get query from sqlFile
        baseSQLFile = open(self._get_absolute_filename('sql/' + self.sql_file))
        sqlString = ''
        for line in baseSQLFile.xreadlines():
            sqlString = sqlString + line
        baseSQLFile.close()

        # Eager free
        testSQLFile.write( "--with eager_free \n" )
        testSQLFile.write( "set gp_resqueue_memory_policy=eager_free;" )
        testSQLFile.write( "\n\n" )
        for statement_mem in self.statement_mem_list:
            testSQLFile.write( "--statement_mem=" + statement_mem.strip() + "\n" )
            testSQLFile.write( "set statement_mem='" + statement_mem.strip() + "';\n" )
            testSQLFile.write( "\n\n" )
            testSQLFile.write( "explain analyze" + "\n" + sqlString )
            testSQLFile.write( "\n\n" )

        # Auto
        testSQLFile.write( "--with auto \n" )
        testSQLFile.write( "set gp_resqueue_memory_policy=auto;" )
        testSQLFile.write( "\n\n" )
        for statement_mem in self.statement_mem_list:
            testSQLFile.write( "--statement_mem=" + statement_mem.strip() + "\n" )
            testSQLFile.write( "set statement_mem='" + statement_mem.strip() + "';\n" )
            testSQLFile.write( "\n\n" )
            testSQLFile.write( "explain analyze" + "\n" + sqlString )
            testSQLFile.write( "\n\n" )


        testSQLFile.close()


    def run_test(self):
        """ This runs the given query with eager_free and auto with all values of statement_mem provided. For verification, we compare the run times with Eager-free and Auto memory policies """
        sql_file = self.sql_file
        ans_file = self.ans_file
        
        test_name = self.name.split('.')[0]
        tinctest.logger.info('Eager_Free: Running test: ' + test_name)

        e_dict = dict.fromkeys(list(x.strip() for x in self.statement_mem_list))
        a_dict = dict.fromkeys(list(x.strip() for x in self.statement_mem_list))

        sql_file = self._get_absolute_filename(test_name + '/' + test_name + '.sql')
        out_file = sql_file[0:len( sql_file ) - 4] + ".out"
  
        # Run setup.sql if exists
        setup_sql_file = self._get_absolute_filename(test_name + '/setup/setup.sql')
        if os.path.exists(setup_sql_file):
            tinctest.logger.info('Eager_Free: Running setup file')
            PSQL.run_sql_file(setup_sql_file, dbname = self.db_name)

        tinctest.logger.info('Eager_Free: Running test case sql ' + sql_file)
        PSQL.run_sql_file(sql_file, dbname = self.db_name, out_file=out_file, flags='-e')

        # Match set statement_mem=
        e_re = re.compile( r"set\s+gp_resqueue_memory_policy\s*=\s*eager_free", re.IGNORECASE )
        a_re = re.compile( r"set\s+gp_resqueue_memory_policy\s*=\s*auto", re.IGNORECASE )
        s_re = re.compile( r"set statement_mem=" , re.IGNORECASE )
        t_re = re.compile( r"Total runtime:" )
        runtime_re = re.compile( r"\d+\.\d+" )

        eager_free = False
        auto = False
        sm = '0MB'

        outFile = open( out_file )
        for line in outFile.xreadlines() :
            if e_re.search( line ) is not None:
                eager_free = True
                auto = False

            if a_re.search( line ) is not None:
                eager_free = False
                auto = True

            sm_match = s_re.search( line )
            if sm_match is not None:
                fields = line.split( '=' )
                sm = fields[1][1:len( fields[1] ) - 3]

            if t_re.search( line ) is not None:
                runtime = runtime_re.search( line ).group()
                if eager_free:
                    e_dict[sm] = runtime
                if auto:
                    a_dict[sm] = runtime

        # Construct an aggregated dictionary
        res = {'eager_free': e_dict, 'auto': a_dict}

        # Result to be returned.
        runtime_stats = {'eager_free':0.0, 'auto':0.0}

        tinctest.logger.info('Eager_Free: Aggregated dictionary : ' + str(res))

        # Write stats to an out file for result-graphing
        out_file = sql_file[0:len( sql_file ) - 4] + "_graph.out"
        outFile = open( out_file, "w" )
        for key in res.keys():
            for statement_mem in self.statement_mem_list:
                # Assert that every query executed successfully. This will be 'None' if there was a crash
                statement_mem = statement_mem.strip()
                self.failUnless( res[key][statement_mem] is not None, "Query with memory_policy=" + key + " and statement_mem=" + statement_mem + "did not execute successfully" )
                runtime_stats[key] = runtime_stats[key] + float( res[key][statement_mem] )
                outFile.write( key + "," + statement_mem + "," + res[key][statement_mem] + "\n" )
        outFile.close()

        tinctest.logger.info('Eager_Free: Runtime stats: ' + str(runtime_stats))
        
        isFail = False
        percentDiff = 0.0
      
        if runtime_stats['eager_free'] > runtime_stats['auto']:
            percentDiff = (runtime_stats['eager_free'] - runtime_stats['auto'])/runtime_stats['auto'] * 100
            if percentDiff > 2.50:
                isFail = True

        self.failUnless( isFail == False, "Eager free took more time than auto" )

        return True
