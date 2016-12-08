import tinctest

from gppylib.commands.base import Command
import mpp.lib.PgHba as PgHba
from mpp.models import SQLTestCase

class ResqueueTest(SQLTestCase):
    '''
    @tags bfv MPP-16059 spi memory_management mediamind
    @author jsoedomo
    @product_version gpdb: 4.3.2.0A, 4.3.2.0ORCA1, [4.3.3.0-], [5.0-], hawq: [1.2.2-]
    @dbname testdb_mpp16059
    '''

    sql_dir = 'sql/'
    ans_dir = 'expected/'
    out_dir = 'output/'
    
    @classmethod
    def setUpClass(cls):
        # Add users to pg_hba
        tinctest.logger.info('Adding users to pg_hba.conf')
        pghba_file = PgHba.PgHba()
        new_ent = PgHba.Entry( entry_type = 'host', database = 'testdb_mpp16059', user = 'all', address = '0.0.0.0/0', authmethod = 'trust' )
        pghba_file.add_entry( new_ent )
        new_ent_1 = PgHba.Entry( entry_type = 'local', database = 'testdb_mpp16059', user = 'all', authmethod = 'trust' )
        pghba_file.add_entry( new_ent_1 )
        pghba_file.write()

        # Restart DB
        Command('Restart database for GUCs to take effect', 
                'source $GPHOME/greenplum_path.sh && gpstop -u').run(validateAfter=True)
