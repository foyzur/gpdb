--query with a shallow group tree. The number of groups and memory intensive operators is same as query1
(
select l_returnflag,l_linestatus,l_partkey,
        sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey
order by l_partkey
)

UNION ALL

(
select l_returnflag,l_linestatus,l_partkey,
       sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1997-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey
order by l_partkey
)

UNION ALL

(
select l_returnflag,l_linestatus,l_partkey,
       sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
   l_shipdate <= date '1999-12-01' - interval '106 day'
group by
   l_returnflag, l_linestatus,l_partkey
order by l_partkey
);
