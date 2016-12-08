--start_ignore
drop table if exists testao;
--end_ignore
create table testao(name integer, id integer, id1 integer, id2 integer) with (appendonly=true);
insert into testao select i, i, i%10, i%5 from generate_series(1, 1000000)i;
