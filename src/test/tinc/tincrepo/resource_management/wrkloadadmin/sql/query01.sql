-- Test Workload Administration and Resource queuing

-- create resource queue
CREATE RESOURCE QUEUE adhoc11 ACTIVE THRESHOLD 1;

-- select from pg_resqueue table
select * from pg_resqueue where rsqname='adhoc11';

--create role and assign role to resource queue
CREATE ROLE role11 with LOGIN RESOURCE QUEUE adhoc11;

-- select role, resource queue details from pg_roles and pg_resqueue tables
SELECT rolname, rsqname FROM pg_roles AS r,pg_resqueue AS q WHERE r.rolresqueue=q.oid and rolname='role11';

-- drop role
DROP ROLE role11;

-- drop resource queue
DROP RESOURCE QUEUE adhoc11;
