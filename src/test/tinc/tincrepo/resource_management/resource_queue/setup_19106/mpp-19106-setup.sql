--start_ignore
drop role u1;
drop resource queue q1;
drop resource queue q2;
--end_ignore
create resource queue q1 with (active_statements = 5);
create resource queue q2 with (active_statements = 5);
create role u1 with login resource queue q1;
