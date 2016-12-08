from mpp.models import SQLTestCase
import tinctest, os, commands

class test_memory_account(SQLTestCase):
    '''
    Includes testing the fix for 
    MPP-21402: Add per-QE/QD memory consumption in CSV dump
    MPP-21576: Create memory accounts for planner, optimizer and plan serializer
    '''
    sql_dir = 'sql/'
    ans_dir = 'expected/'
    out_dir = 'output/'
    
    def verify_out_file(self, out_file, ans_file):

        result = False
        
        # check memory consumption csv file exists in master
        mdd = os.environ.get('MASTER_DATA_DIRECTORY', None)
        if not mdd:
            tinctest.logger.warning("MASTER_DATA_DIRECTORY is not set.")
            tinctest.logger.trace_out()
            return False
        
        log_path = os.path.join(mdd, 'pg_log')
        err_log_message = 'Memory consumption csv file is not generated.'
        for fi in os.listdir(log_path):
            if fi.startswith('memory_'):
                fi_path = os.path.join(log_path, fi)
                is_orca_plan = True
                if out_file.find('planner') != -1:
                    is_orca_plan = False
                result = self.__check_memory_account(fi_path, is_orca_plan)
                if not result:
                    err_log_message = 'Serializer, orca, or planner account is not found in memory consumption csv file.'
        if not result:
            tinctest.logger.warning(err_log_message)
            tinctest.logger.trace_out()
            return False
        
        # check memory consumption csv file exists in segment
        out_dir = self.get_out_dir()
        
        # rm memory consumption file if exists
        rm_cmd = 'rm %smemory_*'%(out_dir)
        commands.getstatusoutput(rm_cmd)
        
        # get primary segment host and fselocation information
        cmd = 'psql postgres -c "select hostname, fselocation from pg_filespace_entry, gp_segment_configuration where dbid = fsedbid and role = \'p\' and content != -1;"'
        status, output = commands.getstatusoutput(cmd)
        if status != 0:
            tinctest.logger.warning('Failed to get segment host and fselocation information.')
            tinctest.logger.trace_out()
            return False
        
        result = []
        pairs = []
        lines = output.split('\n')
        for num in range(len(lines)):
            cur_line = lines[num]
            if num == 0:
                if cur_line.find('hostname') == -1:
                    tinctest.logger.warning('Failed to get segment host and fselocation information.')
                    tinctest.logger.trace_out()
                    return False               
            else:
                if cur_line.find('|')!=-1:
                    # this is a host | fselocation line
                    host = cur_line[:cur_line.index('|')].strip()
                    fselocation = cur_line[cur_line.index('|')+1:].strip()
                    result.append('%s:%s/pg_log/memory_*'%(host, fselocation))
                    pairs.append((host, fselocation))
        
        for per_seg in result:
            # copy memory consumption csv file here
            rsync_cmd = 'rsync -arv %s %s'%(per_seg, out_dir)
            commands.getstatusoutput(rsync_cmd)
            contains_memory_consumption_file = False
            for file in os.listdir(out_dir):
                if file.startswith('memory_'):
                    contains_memory_consumption_file = True
                    break
            if not contains_memory_consumption_file:
                tinctest.logger.warning('Failed to generate memory consumption file in %s.'%per_seg)
                tinctest.logger.trace_out()
                return False
            # rm memory consumption file if exists
            commands.getstatusoutput(rm_cmd)
            
        # rm segment memory consumption file
        rm_seg_cmd = 'gpssh -h %s -e "rm %s/pg_log/memory_*"'
        for pair in pairs:
            commands.getstatusoutput(rm_seg_cmd%(pair[0],pair[1]))
            
        return True
                
    def __check_memory_account(self, memory_file, is_orca_plan):
        check_serializer = False
        check_orca_or_planner = False
        serializer_id = '107'
        orca_or_planner_id = '105'
        if not is_orca_plan:
            orca_or_planner_id = '104'
    
        lines = [line.strip() for line in open(memory_file)]
        for line in lines:
            tokens = line.split(',')
            if tokens[9] == serializer_id:
                check_serializer = True
            elif tokens[9] == orca_or_planner_id:
                check_orca_or_planner = True         
        return check_serializer and check_orca_or_planner;

