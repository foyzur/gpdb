-- Test Workload Administration and Resource queuing

-- create resource queue
CREATE RESOURCE QUEUE adhoc12 ACTIVE THRESHOLD 2;

-- select from pg_resqueue table
select * from pg_resqueue where rsqname='adhoc12';

-- drop resource queue
DROP RESOURCE QUEUE adhoc12;

-- select from pg_resqueue table
--select * from pg_resqueue;
