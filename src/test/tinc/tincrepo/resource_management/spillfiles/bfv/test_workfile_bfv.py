import tinctest

from qautils.gppylib.commands.base import Command

from mpp.models import SQLTestCase

'''
Test Workfile Manager BFVs
'''

class WorkfileBFVTestCase(SQLTestCase):
    """
    @db_name wfc_db
    @product_version hawq: [1.3.0.1-]
    """

    sql_dir = 'sql/'
    ans_dir = 'expected/'
    out_dir = 'output/'
