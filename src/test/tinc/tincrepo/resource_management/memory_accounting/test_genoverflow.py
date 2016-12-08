import tinctest

from mpp.models import MPPTestCase
from mpp.lib.PSQL import PSQL

from gppylib.commands.base import Command
from tinctest.models.scenario import ScenarioTestCase

class GenOverflowTestCase(MPPTestCase, ScenarioTestCase):
    """
    @product_version gpdb:[4.3.0.0-MAIN], hawq: [1.2.1.0-]
    """

    def gp_version(self):
        """
        @todo: ScenarioTest does not have product from MPPTestCase, need to have the method in ScenarioTestCase.
        This is only a hack.
        """
        result = PSQL.run_sql_command( sql_cmd='select version()', flags='-t -q' )
        if "HAWQ" in result:
            return "hawq"
        else:
            return "gpdb"

    def test_01_genoverflow(self):
        """
        @description Runs a simple query from the same session large enough times to overflow the generation, and trigger the generation migration process
        """
        test_case_list1 = []
        test_case_list1.append('resource_management.memory_accounting.scenario.oom_test.runsql.runtest.test_genoverflow')
        self.test_case_scenario.append(test_case_list1)

        test_case_list2 = []
        prd = "_hawq"
        if self.gp_version() == "gpdb":
            prd = ""
        test_case_list2.append('resource_management.memory_accounting.scenario.oom_test.runsql.runtest.test_verify_genoverflow%s' % prd)
        self.test_case_scenario.append(test_case_list2)
