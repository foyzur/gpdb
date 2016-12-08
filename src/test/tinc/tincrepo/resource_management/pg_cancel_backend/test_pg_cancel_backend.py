from resource_management.pg_cancel_backend import PgCancelBackendTestCaseSQLTestCase

class PgCancelBackendTestCase(PgCancelBackendTestCaseSQLTestCase):
    """
    @db_name gptest
    @product_version gpdb: [4.2.6.0-9.9.99.99] , hawq: [1.1.4.0-9.9.99.99]
    @skip Skipping these tests because they are not deterministic.
    """
    sql_dir = 'sql/'
    ans_dir = 'expected/'
    out_dir = 'output/'
