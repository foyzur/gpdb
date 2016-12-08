-- Test Workload Administration and Resource queuing

-- create resource queue
CREATE RESOURCE QUEUE camy11 COST THRESHOLD 200.0;
CREATE RESOURCE QUEUE camy22 COST THRESHOLD 500.0;


-- select from pg_resqueue table
select * from pg_resqueue_status where rsqname in ('camy11','camy22');

--create role and assign role to resource queue
CREATE ROLE creig11 with LOGIN RESOURCE QUEUE camy11;
CREATE ROLE creig22 with LOGIN RESOURCE QUEUE camy22;

-- ALTER Resource Queue
ALTER ROLE creig11 RESOURCE QUEUE camy11;
ALTER ROLE creig22 RESOURCE QUEUE camy22;

-- drop role
DROP ROLE creig11;
DROP ROLE creig22;

-- drop resource queue
DROP RESOURCE QUEUE camy11;
DROP RESOURCE QUEUE camy22;

-- select from pg_resqueue_status table
--select * from pg_resqueue_status;
