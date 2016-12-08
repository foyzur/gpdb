--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';
