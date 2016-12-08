import tinctest
import os

from gppylib.commands.base import Command
import mpp.lib.PgHba as PgHba
from tinctest.models.scenario import ScenarioTestCase

class test_qpri(ScenarioTestCase):
    '''
    @author ramans2
    @created 2013-12-18 12:00:00
    @modified 2013-12-18 12:00:00
    '''
    @classmethod
    def setUpClass(cls):
        # Add users to pg_hba
        tinctest.logger.info('Adding users to pg_hba.conf')
        pghba_file = PgHba.PgHba()
        new_ent = PgHba.Entry( entry_type = 'host', database = 'memory_quota', user = 'all', address = '0.0.0.0/0', authmethod = 'trust' )
        pghba_file.add_entry( new_ent )
        new_ent_1 = PgHba.Entry( entry_type = 'local', database = 'memory_quota', user = 'all', authmethod = 'trust' )
        pghba_file.add_entry( new_ent_1 )
        pghba_file.write()

        # Set up GUC gp_resqueue_priority_cpucores_per_segment
        tinctest.logger.info('Setting GUC gp_resqueue_priority_cpucores_per_segment and restarting DB')
        Command('Run gpconfig to set GUC gp_resqueue_priority_cpucores_per_segment' ,
                'source $GPHOME/greenplum_path.sh;gpconfig -c gp_resqueue_priority_cpucores_per_segment -m 4 -v 2').run(validateAfter=True)

        # Restart DB
        Command('Restart database for GUCs to take effect', 
                'source $GPHOME/greenplum_path.sh && gpstop -ar').run(validateAfter=True)

    def test_compare_max_min(self):
        """ Runs 5 concurrent queries (6 active slices each) - 1 with priority max and the rest with priority min. Verification involves checking that max priority query completed first """
        execorder = self._get_absolute_filename('scenarios/verification/' + 'execorder.out')
        if os.path.exists(execorder):
            os.remove(execorder)

        test_case_list1 = []
        test_case_list1.append("resource_management.query_prioritization.scenarios.setup.create_roles_queues.create_roles_queues.test_setup_max_min")
        self.test_case_scenario.append(test_case_list1)

        test_case_list2 = []
        test_case_list2.append("resource_management.query_prioritization.scenarios.runsql_max_min.runsql.runsql")
        self.test_case_scenario.append(test_case_list2)

        test_case_list3 = []
        test_case_list3.append("resource_management.query_prioritization.scenarios.verification.verify_max.verify")
        self.test_case_scenario.append(test_case_list3)

        test_case_list4 = []
        test_case_list4.append("resource_management.query_prioritization.scenarios.cleanup.drop_roles_queues.drop_roles_queues.test_cleanup_max_min")
        self.test_case_scenario.append(test_case_list4)

    def test_compare_max_low(self):
        """ Runs 5 concurrent queries (6 active slices each) - 1 with priority max and the rest with priority low. Verification involves checking that max priority query completed first """
        execorder = self._get_absolute_filename('scenarios/verification/' + 'execorder.out')
        if os.path.exists(execorder):
            os.remove(execorder)

        test_case_list1 = []
        test_case_list1.append("resource_management.query_prioritization.scenarios.setup.create_roles_queues.create_roles_queues.test_setup_max_low")
        self.test_case_scenario.append(test_case_list1)

        test_case_list2 = []
        test_case_list2.append("resource_management.query_prioritization.scenarios.runsql_max_low.runsql.runsql")
        self.test_case_scenario.append(test_case_list2)

        test_case_list3 = []
        test_case_list3.append("resource_management.query_prioritization.scenarios.verification.verify_max.verify")
        self.test_case_scenario.append(test_case_list3)

        test_case_list4 = []
        test_case_list4.append("resource_management.query_prioritization.scenarios.cleanup.drop_roles_queues.drop_roles_queues.test_cleanup_max_low")
        self.test_case_scenario.append(test_case_list4)

    def test_compare_max_medium(self):
        """ Runs 5 concurrent queries (6 active slices each) - 1 with priority max and the rest with priority medium. Verification involves checking that max priority query completed first """
        execorder = self._get_absolute_filename('scenarios/verification/' + 'execorder.out')
        if os.path.exists(execorder):
            os.remove(execorder)

        test_case_list1 = []
        test_case_list1.append("resource_management.query_prioritization.scenarios.setup.create_roles_queues.create_roles_queues.test_setup_max_medium")
        self.test_case_scenario.append(test_case_list1)

        test_case_list2 = []
        test_case_list2.append("resource_management.query_prioritization.scenarios.runsql_max_medium.runsql.runsql")
        self.test_case_scenario.append(test_case_list2)

        test_case_list3 = []
        test_case_list3.append("resource_management.query_prioritization.scenarios.verification.verify_max.verify")
        self.test_case_scenario.append(test_case_list3)

        test_case_list4 = []
        test_case_list4.append("resource_management.query_prioritization.scenarios.cleanup.drop_roles_queues.drop_roles_queues.test_cleanup_max_medium")
        self.test_case_scenario.append(test_case_list4)

    def test_compare_xhigh_min(self):
        """ Runs 5 concurrent queries (6 active slices each) - 1 with priority high and the rest with priority min. Verification involves checking that high priority query completed first """
        execorder = self._get_absolute_filename('scenarios/verification/' + 'execorder.out')
        if os.path.exists(execorder):
            os.remove(execorder)

        test_case_list1 = []
        test_case_list1.append("resource_management.query_prioritization.scenarios.setup.create_roles_queues.create_roles_queues.test_setup_high_min")
        self.test_case_scenario.append(test_case_list1)

        test_case_list2 = []
        test_case_list2.append("resource_management.query_prioritization.scenarios.runsql_high_min.runsql.runsql")
        self.test_case_scenario.append(test_case_list2)

        test_case_list3 = []
        test_case_list3.append("resource_management.query_prioritization.scenarios.verification.verify_high.verify")
        self.test_case_scenario.append(test_case_list3)

        test_case_list4 = []
        test_case_list4.append("resource_management.query_prioritization.scenarios.cleanup.drop_roles_queues.drop_roles_queues.test_cleanup_high_min")
        self.test_case_scenario.append(test_case_list4)

    def test_compare_xhigh_xlow(self):
        """ Runs 5 concurrent queries (6 active slices each) - 1 with priority high and the rest with priority low. Verification involves checking that high priority query completed first """
        execorder = self._get_absolute_filename('scenarios/verification/' + 'execorder.out')
        if os.path.exists(execorder):
            os.remove(execorder)

        test_case_list1 = []
        test_case_list1.append("resource_management.query_prioritization.scenarios.setup.create_roles_queues.create_roles_queues.test_setup_high_low")
        self.test_case_scenario.append(test_case_list1)

        test_case_list2 = []
        test_case_list2.append("resource_management.query_prioritization.scenarios.runsql_high_low.runsql.runsql")
        self.test_case_scenario.append(test_case_list2)

        test_case_list3 = []
        test_case_list3.append("resource_management.query_prioritization.scenarios.verification.verify_high.verify")
        self.test_case_scenario.append(test_case_list3)

        test_case_list4 = []
        test_case_list4.append("resource_management.query_prioritization.scenarios.cleanup.drop_roles_queues.drop_roles_queues.test_cleanup_high_low")
        self.test_case_scenario.append(test_case_list4)

    def test_changepriority_max_min(self):
        """ Runs 4 concurrent queries (6 active slices each) with priority min. Test changes priority of one active query to max and verifies that the max priority query completes first """
        execorder = self._get_absolute_filename('scenarios/verification/' + 'execorder.out')
        if os.path.exists(execorder):
            os.remove(execorder)

        test_case_list1 = []
        test_case_list1.append("resource_management.query_prioritization.scenarios.setup.create_roles_queues.create_roles_queues.test_setup_changepriority_max_min")
        self.test_case_scenario.append(test_case_list1)

        test_case_list2 = []
        test_case_list2.append("resource_management.query_prioritization.scenarios.changepriority_max_min.runsql.runsql")
        self.test_case_scenario.append(test_case_list2)

        test_case_list3 = []
        test_case_list3.append("resource_management.query_prioritization.scenarios.verification.verify_max_changepriority.verify")
        self.test_case_scenario.append(test_case_list3)

        test_case_list4 = []
        test_case_list4.append("resource_management.query_prioritization.scenarios.cleanup.drop_roles_queues.drop_roles_queues.test_cleanup_changepriority_max_min")
        self.test_case_scenario.append(test_case_list4)

        # Restart DB
        Command('Restart database for GUCs to take effect',
                'source $GPHOME/greenplum_path.sh && gpstop -ar').run(validateAfter=True)

