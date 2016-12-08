--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;

