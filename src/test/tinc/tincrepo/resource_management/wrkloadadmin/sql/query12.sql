-- Test Workload Administration and Resource queuing

-- create resource queue
CREATE RESOURCE QUEUE myq11 ACTIVE THRESHOLD 10;

-- ALTER RESOURCE QUEUE
ALTER RESOURCE QUEUE myq11 COST THRESHOLD 50.0;
ALTER RESOURCE QUEUE myq11 COST THRESHOLD 3e+7;

-- select from pg_resqueue table
select * from pg_resqueue_status where rsqname='myq11';

-- drop resource queue
DROP RESOURCE QUEUE myq11;

-- select from pg_resqueue_status table
--select * from pg_resqueue_status;



