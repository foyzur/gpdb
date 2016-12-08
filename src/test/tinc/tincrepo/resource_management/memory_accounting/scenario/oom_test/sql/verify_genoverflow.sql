-- @author ramans2
-- @created 2014-03-25 12:00:00
-- @modified 2014-03-25 12:00:00
-- @description Check master log to verify generation overflow

select logseverity, logstate, logmessage from gp_toolkit.__gp_log_master_ext where logmessage like 'Migrating all allocated memory chunks to generation%' order by logtime desc limit 1;

-- SQL to check segment logs for ERROR or PANIC messages
select logseverity, logstate, logmessage from gp_toolkit.__gp_log_segment_ext where logstate = 'XX000' and  logtime >= (select logtime from gp_toolkit.__gp_log_master_ext where logmessage like 'statement: select count(1) from test_gen_overflow;' order by logtime desc limit 1) order by logtime desc limit 1;
