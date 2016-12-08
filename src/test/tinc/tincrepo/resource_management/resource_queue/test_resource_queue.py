from tinctest.models.scenario import ScenarioTestCase
from mpp.models import MPPTestCase

class test_resource_queue(MPPTestCase, ScenarioTestCase):
    '''
    @author ramans2
    @created 2013-09-23 12:00:00
    @modified 2013-11-11 12:00:00
    @product_version gpdb:[4.2.7.1-MAIN]
    @tags bfv MPP-19106 MPP-21630
    '''

    @classmethod
    def setUpClass(cls):
        pass

    def test_mpp20483(self):
        '''
        @description MPP-20483: EYEBLASTER - select in cursor is not controlled by resource queue
        '''
        test_case_list1 = []
        test_case_list1.append("resource_management.resource_queue.setup.setup.Setup")
        self.test_case_scenario.append(test_case_list1)

        test_case_list2 = []
        test_case_list2.append("resource_management.resource_queue.runsql.runsql.RunSql")
        self.test_case_scenario.append(test_case_list2)

    def test_mpp19106(self):
        '''
        @description MPP-19106: LWLocks not being released in Resource queue error paths
        '''
        test_case_list1 = []
        test_case_list1.append("resource_management.resource_queue.setup_19106.setup_19106.Setup")
        self.test_case_scenario.append(test_case_list1)

        test_case_list2 = []
        test_case_list2.append("resource_management.resource_queue.runsql_19106.runsql_19106.RunSql")
        self.test_case_scenario.append(test_case_list2)

