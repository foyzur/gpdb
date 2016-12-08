-- Test Workload Administration and Resource queuing

-- create resource queue
CREATE RESOURCE QUEUE tom55 ACTIVE THRESHOLD 10;

--create role and assign role to resource queue
CREATE ROLE shaun55 with LOGIN RESOURCE QUEUE tom55;

-- select from pg_resqueue, pg_role table
SELECT rolname, rsqname, pid, granted, current_query,datname FROM pg_roles, pg_resqueue, pg_locks, pg_stat_activity WHERE pg_roles.rolresqueue=pg_locks.objid AND pg_locks.objid=pg_resqueue.oid AND pg_stat_activity.procpid=pg_locks.pid;

-- create a view
CREATE VIEW resq_procs AS SELECT rolname, rsqname, pid, granted, current_query,datname FROM pg_roles, pg_resqueue, pg_locks, pg_stat_activity WHERE pg_roles.rolresqueue=pg_locks.objid AND pg_locks.objid=pg_resqueue.oid AND pg_stat_activity.procpid=pg_locks.pid;

-- select from view
select * from resq_procs;

-- drop role name
DROP ROLE shaun55;

-- drop resource queue
DROP RESOURCE QUEUE tom55;

-- select from view
--select * from resq_procs;

-- drop view
DROP VIEW resq_procs;


