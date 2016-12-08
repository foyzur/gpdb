from resource_management.memory_quota.eager_free import EagerFreeSQLTestCase
from mpp.models import SQLTestCase
from gppylib.commands.base import Command

class test71_eager_free_query1_deep_plan(EagerFreeSQLTestCase):
    """
    @db_name memory_quota
    @sql_file e_query1.sql 
    @statement_mem 75MB, 125MB, 175MB, 225MB, 275MB, 325MB, 375MB, 425MB, 475MB, 525MB
    @description Deep group tree with mostly hashaggs increasing the number of groups as we go deeper in the query plan
    """
    sql_dir = 'test71_eager_free_query1_deep_plan'
    ans_dir = 'test71_eager_free_query1_deep_plan'
    out_dir = 'test71_eager_free_query1_deep_plan'

class test72_eager_free_query2_deep_plan_with_more_MI_operators(EagerFreeSQLTestCase):
    """
    @db_name memory_quota
    @sql_file e_query2.sql
    @statement_mem 75MB, 125MB, 175MB, 225MB, 275MB, 325MB, 375MB, 425MB, 475MB, 525MB
    @description Deep group tree with more number of memory intensive operators, increasing the number of groups as we go deeper in the query plan
    """
    sql_dir = 'test72_eager_free_query2_deep_plan_with_more_MI_operators'
    ans_dir = 'test72_eager_free_query2_deep_plan_with_more_MI_operators'
    out_dir = 'test72_eager_free_query2_deep_plan_with_more_MI_operators'

class test73_eager_free_query3_mark_restore(EagerFreeSQLTestCase):
    """
    @db_name memory_quota
    @sql_file e_query3.sql
    @statement_mem 75MB, 125MB, 175MB, 225MB, 275MB, 325MB, 375MB, 425MB, 475MB, 525MB
    @config_params enable_hashjoin=off, enable_mergejoin=on
    @description Queries with mark/restore and blocking operators below
    """
    sql_dir = 'test73_eager_free_query3_mark_restore'
    ans_dir = 'test73_eager_free_query3_mark_restore'
    out_dir = 'test73_eager_free_query3_mark_restore'

class test74_eager_free_query4_shallow_group_tree(EagerFreeSQLTestCase):
    """
    @db_name memory_quota
    @sql_file e_query4.sql
    @statement_mem 75MB, 125MB, 175MB, 225MB, 275MB, 325MB, 375MB, 425MB, 475MB, 525MB
    @config_params enable_hashjoin=on
    @description With group tree being shallow and the same number of groups and operators as query2
    """
    sql_dir = 'test74_eager_free_query4_shallow_group_tree'
    ans_dir = 'test74_eager_free_query4_shallow_group_tree'
    out_dir = 'test74_eager_free_query4_shallow_group_tree'

class test75_eager_free_query5_rescan_operators(EagerFreeSQLTestCase):
    """
    @db_name memory_quota
    @sql_file e_query5.sql
    @statement_mem 75MB, 125MB, 175MB, 225MB, 275MB, 325MB, 375MB, 425MB, 475MB, 525MB
    @config_params enable_hashjoin=on, enable_seqscan=off, enable_indexscan=on, enable_bitmapscan=on
    @description With blocking memory intensive operators requiring rescan that should not be considered blocking. Using bitmap index scan
   """
    @classmethod
    def setUpClass(cls):
        # Restart DB
        Command('Restart database for GUCs to take effect',
                'source $GPHOME/greenplum_path.sh && gpstop -ar').run(validateAfter=True)

    sql_dir = 'test75_eager_free_query5_rescan_operators'
    ans_dir = 'test75_eager_free_query5_rescan_operators'
    out_dir = 'test75_eager_free_query5_rescan_operators'


class test_misc_eager_free(SQLTestCase):
    """
    @db_name eager_free
    @author gcaragea
    @created 2014-08-13 12:00:00
    @modified 2014-08-13 12:00:00
    @description Miscelaneous eager free BFVs
    """
    
    sql_dir = 'sql'
    ans_dir = 'ans'
    out_dir = 'out'
