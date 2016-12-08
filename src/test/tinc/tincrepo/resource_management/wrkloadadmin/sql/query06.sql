-- Test Workload Administration and Resource queuing

-- create resource queue and assuming that 8 is the maximum limit for the resource queue and 1 resource queue is already present in the table
-- it will give error if it crosses the maximum limit of resource queue
CREATE RESOURCE QUEUE adhoc1 ACTIVE THRESHOLD 1;
CREATE RESOURCE QUEUE webuser1 ACTIVE THRESHOLD 3;
CREATE RESOURCE QUEUE mgmtuser1 ACTIVE THRESHOLD 5;
CREATE RESOURCE QUEUE mgmtuser2 ACTIVE THRESHOLD 7;
CREATE RESOURCE QUEUE mgmtuser3 ACTIVE THRESHOLD 9;
CREATE RESOURCE QUEUE mgmtuser4 ACTIVE THRESHOLD 8;
CREATE RESOURCE QUEUE mgmtuser5 ACTIVE THRESHOLD 6;
CREATE RESOURCE QUEUE mgmtuser6 ACTIVE THRESHOLD 2;

-- select from pg_resqueue table
select * from pg_resqueue;

-- drop resource queue
DROP RESOURCE QUEUE adhoc1;
DROP RESOURCE QUEUE webuser1;
DROP RESOURCE QUEUE mgmtuser1;
DROP RESOURCE QUEUE mgmtuser2;
DROP RESOURCE QUEUE mgmtuser3;
DROP RESOURCE QUEUE mgmtuser4;
DROP RESOURCE QUEUE mgmtuser5;
DROP RESOURCE QUEUE mgmtuser6;

-- select from pg_resqueue table
select * from pg_resqueue;
