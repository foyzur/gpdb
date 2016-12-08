-- Test Workload Administration and Resource queuing

-- create resource queue
CREATE RESOURCE QUEUE tom11 ACTIVE THRESHOLD 20;

--create role and assign role to resource queue
CREATE ROLE shaun11 with LOGIN RESOURCE QUEUE tom11;

-- select from pg_resqueue, pg_roles table
SELECT rolname, rsqname FROM pg_roles, pg_resqueue WHERE pg_roles.rolresqueue=pg_resqueue.oid and rolname='shaun11';

-- create a view
CREATE VIEW role2que AS SELECT rolname, rsqname FROM pg_roles, pg_resqueue WHERE pg_roles.rolresqueue=pg_resqueue.oid and rolname='shaun11';

-- select from view
select * from role2que where rolname='shaun11';

-- drop role name
DROP ROLE shaun11;

-- drop resource queue
DROP RESOURCE QUEUE tom11;

-- select from view
--select * from role2que;

-- drop view
DROP VIEW role2que;

