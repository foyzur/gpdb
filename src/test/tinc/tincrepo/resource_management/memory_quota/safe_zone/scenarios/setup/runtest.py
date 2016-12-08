from mpp.models import SQLConcurrencyTestCase
from mpp.models import SQLTestCase

class runsql_concurrency(SQLConcurrencyTestCase):
    """
    @db_name memory_quota_1GB
    @gpdiff True
    """
    pass

class runsql_concurrency_nodiff(SQLConcurrencyTestCase):
    """
    @db_name memory_quota_1GB
    @gpdiff False
    """
    pass

class runsql(SQLTestCase):
    """
    @db_name memory_quota_1GB
    @gpdiff False
    """
    sql_dir = 'sql/'
