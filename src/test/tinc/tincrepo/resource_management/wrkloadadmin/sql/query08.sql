-- Test Workload Administration and Resource queuing

-- create resource queue
CREATE RESOURCE QUEUE sameera ACTIVE THRESHOLD 2;

-- select from pg_resqueue table
select * from pg_resqueue where rsqname='sameera';

--create role and assign role to resource queue
CREATE ROLE aryan with LOGIN RESOURCE QUEUE sameera;

-- ALTER Resource Queue
ALTER ROLE aryan RESOURCE QUEUE none;

-- select role, resource queue details from pg_roles and pg_resqueue tables
SELECT rolname, rsqname FROM pg_roles AS r,pg_resqueue AS q WHERE r.rolresqueue=q.oid and rolname='aryan';

-- drop role
DROP ROLE aryan;

-- drop resource queue
DROP RESOURCE QUEUE sameera;

-- select from pg_resqueue table
--select * from pg_resqueue;
