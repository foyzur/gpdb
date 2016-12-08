drop table if exists mpp21402;

create table mpp21402(c1 int, c2 int);

insert into mpp21402 select i, i from generate_series(1,100)i;

