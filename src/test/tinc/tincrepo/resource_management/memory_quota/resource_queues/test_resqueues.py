import tinctest

from gppylib.commands.base import Command
import mpp.lib.PgHba as PgHba
from tinctest.models.scenario import ScenarioTestCase

class ResqueueTest(ScenarioTestCase):
    '''
    @author ramans2
    @created 2013-12-16 12:00:00
    @modified 2014-04-03 12:00:00
    '''
    @classmethod
    def setUpClass(cls):
        # Add users to pg_hba
        tinctest.logger.info('Adding users to pg_hba.conf')
        pghba_file = PgHba.PgHba()
        new_ent = PgHba.Entry( entry_type = 'host', database = 'memory_quota_1GB', user = 'all', address = '0.0.0.0/0', authmethod = 'trust' )
        pghba_file.add_entry( new_ent )
        new_ent_1 = PgHba.Entry( entry_type = 'local', database = 'memory_quota_1GB', user = 'all', authmethod = 'trust' )
        pghba_file.add_entry( new_ent_1 )
        pghba_file.write()

        # Set up GUC gp_resqueue_memory_policy_auto_fixed_mem
        tinctest.logger.info('Setting GUC gp_resqueue_memory_policy_auto_fixed_mem')
        Command('Run gpconfig to set GUC gp_resqueue_memory_policy_auto_fixed_mem',
                'source $GPHOME/greenplum_path.sh;gpconfig -c gp_resqueue_memory_policy_auto_fixed_mem -v 200kB --skipvalidation').run(validateAfter=True)

        # Restart DB
        Command('Restart database for GUCs to take effect', 
                'source $GPHOME/greenplum_path.sh && gpstop -ar').run(validateAfter=True)

    def test_checkmemoryconsumption(self):
        '''
        @description Check that each query gets 'statement_mem' when no memory limit is specified for the resource queue
        '''
        test_case_list1 = []
        test_case_list1.append("resource_management.memory_quota.resource_queues.scenarios.setup.create_roles_queues.create_roles_queues.test_create_queues")
        self.test_case_scenario.append(test_case_list1)

        test_case_list2 = []
        test_case_list2.append("resource_management.memory_quota.resource_queues.scenarios.runsql_memoryconsumption.runsql.memory_consumption")
        self.test_case_scenario.append(test_case_list2)

    def test_checkquerywaiting(self):
        '''
        @description Check that queries wait when the resource queue is already maxed out on memory
        '''
        test_case_list1 = []
        test_case_list1.append("resource_management.memory_quota.resource_queues.scenarios.setup.create_roles_queues.create_roles_queues.test_create_queues_memorylimit")
        self.test_case_scenario.append(test_case_list1)

        test_case_list2 = []
        test_case_list2.append("resource_management.memory_quota.resource_queues.scenarios.runsql_querywaiting.runsql.query_waiting")
        self.test_case_scenario.append(test_case_list2)

