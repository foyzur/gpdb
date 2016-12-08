from mpp.models import SQLTestCase

class WrkloadadminTestCase(SQLTestCase):
 """
 @db_name gptest
 @product_version gpdb: [4.2.0.0-9.9.99.99]
 @author cdbfast
 @modified 2013-12-18 12:00:00
 @created 2013-12-18 12:00:00
 @tags wrkloadadmin list_queries
 @description cdbfast.wrkloadadmin
 """
 sql_dir = 'sql/'
 ans_dir = 'expected/'
 out_dir = 'output/'
