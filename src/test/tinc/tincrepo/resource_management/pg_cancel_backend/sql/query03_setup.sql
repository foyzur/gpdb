drop table createdata;
create table createdata(a int);
insert into createdata values (generate_series(1,3000));
create table pg_cancel_backend_query03 (a int);


\o /tmp/query3.data
select a.a from createdata a, createdata b;
\o
