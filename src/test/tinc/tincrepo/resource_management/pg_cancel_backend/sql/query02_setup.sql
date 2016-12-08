drop table pg_cancel_backend_query02;
create table pg_cancel_backend_query02(a int, b int, c int, d int);
insert into pg_cancel_backend_query02 values (generate_series(1,100000), generate_series(1,100000), generate_series(1,100000), generate_series(1,100000));
