from mpp.models import SQLTestCase

class MemoryQuotaResultNodeTestCase(SQLTestCase):
    """
    @optimizer_mode on
    Test result node from ORCA is non-memory intensive
    """
    sql_dir = 'sql/'
    ans_dir = 'expected/'
    out_dir = 'output/'