import tinctest
import os
import shutil

import mpp.lib.PgHba as PgHba
from tinctest.lib.system import TINCSystem
from gppylib.commands.base import Command
from mpp.lib.PSQL import PSQL
from tinctest.models.scenario import ScenarioTestCase

class test_dangerzone_memoryquota(ScenarioTestCase):
    """
    @author ramans2
    @created 2014-04-03 12:00:00
    @modified 2014-04-24 12:00:00
    @db_name memory_quota_1GB
    @product_version gpdb:[4.2.0.0-MAIN]
    @description Tests that check for safezone without memory quota
    """
    @classmethod
    def setUpClass(cls):
        super(test_dangerzone_memoryquota, cls).setUpClass()
        # Add users to pg_hba
        tinctest.logger.info('Adding users to pg_hba.conf')
        pghba_file = PgHba.PgHba()
        new_ent = PgHba.Entry( entry_type = 'host', database = 'memory_quota_1GB', user = 'all', address = '0.0.0.0/0', authmethod = 'trust' )
        pghba_file.add_entry( new_ent )
        new_ent_1 = PgHba.Entry( entry_type = 'local', database = 'memory_quota_1GB', user = 'all', authmethod = 'trust' )
        pghba_file.add_entry( new_ent_1 )
        pghba_file.write()

        # Set up GUC max_connections, max_prepared_transactions
        tinctest.logger.info('Setting GUCs max_connections and max_prepared_transactions to be able to handle concurrency')
        Command('Run gpconfig to set GUC max_connections',
                'source $GPHOME/greenplum_path.sh;gpconfig -c max_connections -v 350 -m 300').run(validateAfter=True)
        Command('Run gpconfig to set GUC max_prepared_transactions',
                'source $GPHOME/greenplum_path.sh;gpconfig -c max_prepared_transactions -v 350 -m 300').run(validateAfter=True)
        Command('Run gpconfig to set GUC gp_resqueue_memory_policy_auto_fixed_mem', 
                            'source $GPHOME/greenplum_path.sh;gpconfig -c gp_resqueue_memory_policy_auto_fixed_mem -v 200kB  --skipvalidation').run(validateAfter=True)

        # Restart DB
        Command('Restart database for GUCs to take effect', 
                'source $GPHOME/greenplum_path.sh && gpstop -ar').run(validateAfter=True)

    def _create_roles_queues(self, test_name):
        # Make test directory 
        if os.path.exists(self._get_absolute_filename('./scenarios/' + test_name)):
            shutil.rmtree(self._get_absolute_filename('./scenarios/' + test_name))

        os.makedirs(self._get_absolute_filename('./scenarios/' + test_name))
        os.makedirs(self._get_absolute_filename('./scenarios/' + test_name + '/sql'))
        shutil.copyfile(self._get_absolute_filename('./scenarios/setup/runtest.py'), self._get_absolute_filename('./scenarios/' + test_name + '/runtest.py'))
        open(self._get_absolute_filename('./scenarios/' + test_name + '/__init__.py'), 'a').close()

        substitutions = {}

        if self.memory_limit.lower() == "none":
            substitutions= {'alter resource queue @resource_queue@ with (memory_limit = \'@memory_limit@\');':'\n'}
        else:
            substitutions = {'@memory_limit@':self.memory_limit}
        addtransforms = {'@user_name@':'testuser1',
                         '@resource_queue@':'resourcequeue1',
                         '@concurrency@':self.statement_count}
        substitutions.update(addtransforms)

        TINCSystem.substitute_strings_in_file(self._get_absolute_filename('./scenarios/setup/sql/create_roles_and_queues.sql.t'),
                                                              self._get_absolute_filename('./scenarios/' + test_name + '/sql/create_roles_and_queues.sql'),
                                                              substitutions)

    def _create_memquota_testfiles(self, test_name):
        _user1 = 'testuser1'

        base_sql = open ( self._get_absolute_filename('./scenarios/setup/sql/base.sql') )
        test_sql = open ( self._get_absolute_filename('./scenarios/'+ test_name + '/' + test_name + '.sql'), 'w' )

        connect_string = '\c ' + self.db_name + ' ' + _user1 + ';'
        sql_string = ''
        # Naive concatenation
        for line in base_sql.xreadlines():
            sql_string = sql_string + line

        base_sql.close()

        sql_string = sql_string.rstrip()
        sql_string = sql_string.rstrip( ';' )

        formatted_sql = ''

        for i in range( 1, int( self.complexity ) + 1 ):
            if i == int( self.complexity ):
                formatted_sql = formatted_sql + '\n' + sql_string + ";"
            else:
                formatted_sql = formatted_sql + '\n' + sql_string + "\n" + " UNION "

        test_sql.write( "--@gucs gp_resqueue_memory_policy=" + self.memory_policy + ";statement_mem=\'" + self.statement_mem + "\'\n" )
        test_sql.write( "--@concurrency " + str(self.concurrency_limit) + "\n" )
        test_sql.write( connect_string + "\n" )
        test_sql.write( formatted_sql )
        test_sql.close()

        substitutions = {'@user_name@':_user1, '@dbname@':self.db_name, '@test_query@':formatted_sql}
        TINCSystem.substitute_strings_in_file(self._get_absolute_filename('./scenarios/setup/expected/base.ans.t'),
                                                              self._get_absolute_filename('./scenarios/setup/expected/base.ans'),
                                                              substitutions)

        base_ans_file = open( self._get_absolute_filename('./scenarios/setup/expected/base.ans') )
        test_ans_file = open( self._get_absolute_filename('./scenarios/' + test_name + '/' + test_name + '.ans'), 'w' )
        for line in base_ans_file.xreadlines():
            test_ans_file.write( line )

        base_ans_file.close()
        test_ans_file.close()

    def setUp(self):
        super(test_dangerzone_memoryquota, self).setUp()
        test_name = self.name.split('.')[1]
        # Create sql file to setup roles and queues
        self._create_roles_queues(test_name)
        # Create sql and ans files for memory quota test
        self._create_memquota_testfiles(test_name)
        if self.name == 'test_dangerzone_memoryquota.test_222_dangerzone_memoryquota':
            self.set_vmem_protect(500,500)
        if self.name == 'test_dangerzone_memoryquota.test_223_dangerzone_memoryquota':
            self.set_vmem_protect(400,400)

    def tearDown(self):
        super(test_dangerzone_memoryquota, self).tearDown()
        if self.name == 'test_dangerzone_memoryquota.test_222_dangerzone_memoryquota' or \
           self.name == 'test_dangerzone_memoryquota.test_223_dangerzone_memoryquota':
            self.reset_vmem_protect()

    def reset_vmem_protect(self):
        self.set_vmem_protect()

    def set_vmem_protect(self, vmem_master=None, vmem_segment=None):
        tinctest.logger.info('Setting GUC and restarting DB')
        gpconfig_cmd = "gpconfig -c gp_vmem_protect_limit -m %s -v %s" % (vmem_master, vmem_segment)
        if vmem_master == None and vmem_segment == None:
            gpconfig_cmd = "gpconfig -r gp_vmem_protect_limit" 
        Command('Run gpconfig to set GUC gp_vmem_protect_limit' ,
                'source $GPHOME/greenplum_path.sh; %s' % (gpconfig_cmd)).run(validateAfter=True)
        # Restart DB
        Command('Restart database for GUCs to take effect',
                'source $GPHOME/greenplum_path.sh && gpstop -ar').run(validateAfter=True)

    def _infer_metadata(self):
        """
        Read all the metadata and store them as instance variables.
        """
        super(test_dangerzone_memoryquota, self)._infer_metadata()
        self.db_name = self._metadata.get('db_name', 'memory_quota_1GB')
        self.memory_limit = self._metadata.get('memory_limit', 'None')
        self.statement_count = self._metadata.get('statement_count', 20)
        self.memory_policy = self._metadata.get('memory_policy', 'None')
        self.statement_mem = self._metadata.get('statement_mem', '128MB')
        self.concurrency_limit = self._metadata.get('concurrency_limit', 2)
        self.complexity = self._metadata.get('complexity', 1)

    def test_221_dangerzone_memoryquota(self):
        """ 
        @memory_limit None
        @statement_count 40
        @memory_policy eager_free
        @statement_mem 125MB
        @concurrency_limit 40
        @complexity 1
        @description Check danger zone limit for complexity 1 with memory quota. Should throw OOM
        """
        test_name = self.name.split('.')[1]

        # Run sql to create roles and queues
        test_case_list0 = []
        test_case_list0.append("resource_management.memory_quota.safe_zone.scenarios." + test_name + ".runtest.runsql")
        self.test_case_scenario.append(test_case_list0)

        # Run test case concurrently
        test_case_list1 = []
        test_case_list1.append("resource_management.memory_quota.safe_zone.scenarios." + test_name + ".runtest.runsql_concurrency_nodiff")
        self.test_case_scenario.append(test_case_list1)

        # Verify that at least one request fails with OOM, Verify that when results are returned, they are correct
        test_case_list2 = []
        test_case_list2.append(("resource_management.memory_quota.safe_zone.scenarios.verify.check_files.verify_results", [test_name]))
        self.test_case_scenario.append(test_case_list2)

    def test_222_dangerzone_memoryquota(self):
        """ 
        @memory_limit None
        @statement_count 30
        @memory_policy eager_free
        @statement_mem 125MB
        @concurrency_limit 2
        @complexity 2
        @description Check danger zone limit for complexity 2 with memory quota. Should throw OOM
        """
        test_name = self.name.split('.')[1]

        # Run sql to create roles and queues
        test_case_list0 = []
        test_case_list0.append("resource_management.memory_quota.safe_zone.scenarios." + test_name + ".runtest.runsql")
        self.test_case_scenario.append(test_case_list0)

        # Run test case concurrently
        test_case_list1 = []
        test_case_list1.append("resource_management.memory_quota.safe_zone.scenarios." + test_name + ".runtest.runsql_concurrency_nodiff")
        self.test_case_scenario.append(test_case_list1)

        # Verify that at least one request fails with OOM, Verify that when results are returned, they are correct
        test_case_list2 = []
        test_case_list2.append(("resource_management.memory_quota.safe_zone.scenarios.verify.check_files.verify_results", [test_name]))
        self.test_case_scenario.append(test_case_list2)

    def test_223_dangerzone_memoryquota(self):
        """ 
        @memory_limit None
        @statement_count 30
        @memory_policy eager_free
        @statement_mem 125MB
        @concurrency_limit 2
        @complexity 3
        @description Check danger zone limit for complexity 3 with memory quota. Should throw OOM
        """
        test_name = self.name.split('.')[1]

        # Run sql to create roles and queues
        test_case_list0 = []
        test_case_list0.append("resource_management.memory_quota.safe_zone.scenarios." + test_name + ".runtest.runsql")
        self.test_case_scenario.append(test_case_list0)

        # Run test case concurrently
        test_case_list1 = []
        test_case_list1.append("resource_management.memory_quota.safe_zone.scenarios." + test_name + ".runtest.runsql_concurrency_nodiff")
        self.test_case_scenario.append(test_case_list1)

        # Verify that at least one request fails with OOM, Verify that when results are returned, they are correct
        test_case_list2 = []
        test_case_list2.append(("resource_management.memory_quota.safe_zone.scenarios.verify.check_files.verify_results", [test_name]))
        self.test_case_scenario.append(test_case_list2)

