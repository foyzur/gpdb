drop table if exists smallt_part;
drop table if exists smallt2_part;

create table smallt_part (i int, t text, d date)
partition by range (d) (start ('2011-01-01'::date) end ('2011-01-21'::date) every ('5 days'::interval));

create table smallt2_part (i int, t text, d date)
partition by range (d) (start ('2011-01-01'::date) end ('2011-01-16'::date) every ('4 days'::interval));

insert into smallt_part select i%10, 'text ' || (i%15), '2011-01-01'::date + ((i%20) || ' days')::interval
from generate_series(0, 99) i;

insert into smallt2_part select i%5, 'text ' || (i%10), '2011-01-01'::date + ((i%15) || ' days')::interval
from generate_series(0, 49) i; 
