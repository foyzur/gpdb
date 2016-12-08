-- Test Workload Administration and Resource queuing

-- create resource queue
CREATE RESOURCE QUEUE adhoc1 ACTIVE THRESHOLD 1;
CREATE RESOURCE QUEUE webuser1 ACTIVE THRESHOLD 3;
CREATE RESOURCE QUEUE mgmtuser1 ACTIVE THRESHOLD 5;

-- select from pg_resqueue table
select * from pg_resqueue where rsqname in ('adhoc1','webuser1','mgmtuser1');

-- drop resource queue
DROP RESOURCE QUEUE adhoc1;
DROP RESOURCE QUEUE webuser1;
DROP RESOURCE QUEUE mgmtuser1;


-- select from pg_resqueue table
--select * from pg_resqueue;
