-- Test Workload Administration and Resource queuing

-- create resource queue
CREATE RESOURCE QUEUE bob ACTIVE THRESHOLD 1;

-- select from pg_resqueue table
select * from pg_resqueue where rsqname='bob';

-- ALTER Resource Queue
ALTER RESOURCE QUEUE bob ACTIVE THRESHOLD 7;

-- select from pg_resqueue table
select * from pg_resqueue where rsqname='bob';

-- drop resource queue
DROP RESOURCE QUEUE bob;

-- select from pg_resqueue table
-- select * from pg_resqueue;
