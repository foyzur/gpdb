-- Test Workload Administration and Resource queuing

-- create resource queue
CREATE RESOURCE QUEUE myq21 ACTIVE THRESHOLD 7;

-- ALTER RESOURCE QUEUE
ALTER RESOURCE QUEUE myq21 COST THRESHOLD 70.0;
ALTER RESOURCE QUEUE myq21 COST THRESHOLD 3e+9 NOOVERCOMMIT;

-- select from pg_resqueue table
select * from pg_resqueue_status where rsqname='myq21';

-- drop resource queue
DROP RESOURCE QUEUE myq21;

-- select from pg_resqueue_status table
--select * from pg_resqueue_status;



