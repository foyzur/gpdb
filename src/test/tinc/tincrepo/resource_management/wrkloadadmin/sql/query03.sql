-- Test Workload Administration and Resource queuing

-- create resource queue
CREATE RESOURCE QUEUE adhoc13 ACTIVE THRESHOLD 2;
CREATE RESOURCE QUEUE adhoc14 ACTIVE THRESHOLD 3;

-- select from pg_resqueue table
select * from pg_resqueue where rsqname='adhoc13' and rsqname='adhoc14';

--create role and assign role to resource queue
CREATE ROLE role13 with LOGIN RESOURCE QUEUE adhoc13;
CREATE ROLE role14 with LOGIN RESOURCE QUEUE adhoc14;

-- select role, resource queue details from pg_roles and pg_resqueue tables
SELECT rolname, rsqname FROM pg_roles AS r,pg_resqueue AS q WHERE r.rolresqueue=q.oid and rolname in ('role13','role14');

-- drop role
DROP ROLE IF EXISTS role13;
DROP ROLE IF EXISTS role14;

-- drop resource queue
DROP RESOURCE QUEUE adhoc13;
DROP RESOURCE QUEUE adhoc14;
