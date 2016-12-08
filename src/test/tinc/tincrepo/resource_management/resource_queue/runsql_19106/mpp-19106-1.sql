select pg_sleep(2);
alter role u1 with resource queue q2;
drop resource queue q1;
