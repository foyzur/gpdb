import os

import tinctest
from tinctest.lib.system import TINCSystem

from qautils.gppylib.commands.base import Command

from mpp.lib.PSQL import PSQL
from mpp.models import SQLTestCase

'''
Test Workfile Manager Space accounting Tests
'''

class WorkfileSpaceAccountingTestCase(SQLTestCase):
    """
    @db_name wfc_db
    @product_version gpdb: [4.3.3.0-], [4.2.8.1-4.2], hawq: [1.2.2.0-]
    """

    sql_dir = 'sql/'
    ans_dir = 'expected/'
    out_dir = 'output/'

    @classmethod
    def setUpClass(cls):
        """
        Run the specific setUpClass.sql or setUpClassHAWQ.sql based on the product
        """
	super(WorkfileSpaceAccountingTestCase, cls).setUpClass()
        setup_sql_file = "setUpClass.sql"
        if cls.__product__ == 'hawq':
            setup_sql_file = "setUpClassHAWQ.sql"

        setup_dir = os.path.join(cls.get_sql_dir(), 'setup')
        setup_out_dir = os.path.join(cls.get_out_dir(), 'setup')
        TINCSystem.make_dirs(setup_out_dir, ignore_exists_error = True)

        out_file = os.path.join(setup_out_dir, setup_sql_file.replace('.sql', '.out'))
        setup_sql_file = os.path.join(setup_dir, setup_sql_file)
        tinctest.logger.info("Running setup sql for test - %s" %setup_sql_file)
        PSQL.run_sql_file(setup_sql_file, dbname="wfc_db", out_file=out_file)

        tinctest.logger.info('Running Workfile Space Accounting Tests')
        Command('gpconfig', 'gpconfig -c gp_workfile_limit_per_segment -v "10GB" --skipvalidation').run(validateAfter=True)
        Command('restart HAWQ', 'source $GPHOME/greenplum_path.sh && gpstop -ar').run(validateAfter=True)

    @classmethod
    def tearDownClass(cls):
        """
        Restart DB cluster in normal mode
        """
        super(WorkfileSpaceAccountingTestCase, cls).tearDownClass()
        tinctest.logger.info("Restart DB in normal mode")
        Command('gpconfig', 'gpconfig -r gp_workfile_limit_per_segment --skipvalidation').run(validateAfter=True)
        Command('restart HAWQ', 'source $GPHOME/greenplum_path.sh && gpstop -ar').run(validateAfter=True)
