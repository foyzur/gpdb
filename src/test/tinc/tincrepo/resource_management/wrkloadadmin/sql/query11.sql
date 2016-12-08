-- Test Workload Administration and Resource queuing

-- create resource queue
CREATE RESOURCE QUEUE tom ACTIVE THRESHOLD 20;

-- ALTER RESOURCE QUEUE
ALTER RESOURCE QUEUE tom COST THRESHOLD 100.0;

-- select from pg_resqueue table
select * from pg_resqueue_status where rsqname='tom';

-- drop resource queue
DROP RESOURCE QUEUE tom;

-- select from pg_resqueue_status table
--select * from pg_resqueue_status;



