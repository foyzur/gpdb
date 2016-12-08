-- Test Workload Administration and Resource queuing

-- create resource queue
CREATE RESOURCE QUEUE ram ACTIVE THRESHOLD 1;

-- select from pg_resqueue table
select * from pg_resqueue where rsqname='ram';

--create role and assign role to resource queue
CREATE ROLE sita with LOGIN RESOURCE QUEUE ram;
CREATE ROLE samrat with LOGIN RESOURCE QUEUE ram;

-- ALTER ROLE
ALTER ROLE sita RESOURCE QUEUE ram;
ALTER ROLE samrat RESOURCE QUEUE ram;

-- select role, resource queue details from pg_roles and pg_resqueue tables
SELECT rolname, rsqname FROM pg_roles AS r,pg_resqueue AS q WHERE r.rolresqueue=q.oid and rsqname='ram';

-- drop role
DROP ROLE sita;
DROP ROLE samrat;


-- drop resource queue
DROP RESOURCE QUEUE ram;
