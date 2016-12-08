-- Test Workload Administration and Resource queuing

-- create resource queue
CREATE RESOURCE QUEUE adhoc1 ACTIVE THRESHOLD 1;
CREATE RESOURCE QUEUE adhoc2 ACTIVE THRESHOLD 2;
CREATE RESOURCE QUEUE adhoc3 ACTIVE THRESHOLD 3;
--CREATE RESOURCE QUEUE adhoc4 ACTIVE THRESHOLD 4;

-- select from pg_resqueue table
--select * from pg_resqueue;

-- drop resource queue
DROP RESOURCE QUEUE adhoc1;
DROP RESOURCE QUEUE adhoc2;
DROP RESOURCE QUEUE adhoc3;
--DROP RESOURCE QUEUE adhoc4;

-- select from pg_resqueue table
--select * from pg_resqueue;

