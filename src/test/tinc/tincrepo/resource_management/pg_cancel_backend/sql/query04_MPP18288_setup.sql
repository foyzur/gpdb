drop table if exists pg_cancel_backend_query04_MPP18288; 
create table pg_cancel_backend_query04_MPP18288(i1 int, i2 int, i3 int, i4 int); 
insert into pg_cancel_backend_query04_MPP18288 select i, i % 1000, i % 100000, i % 75 from generate_series(0,99999999) i; 
