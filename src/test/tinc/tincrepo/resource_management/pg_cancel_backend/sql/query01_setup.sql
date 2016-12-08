drop table pg_cancel_backend_query01;
create table pg_cancel_backend_query01 (a int, b int, c int);
insert into pg_cancel_backend_query01 values (generate_series(1,1000), generate_series(1,1000), generate_series(1,1000));
